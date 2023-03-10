//
//  AudioController.swift
//
//  Residuals from Apple's aurioTouch translated by OOPer in cooperation with shlab.jp, on 2015/1/31.
//
//  01/07/2022 Fixed dangling pointer warning (contributor: Toby Bell)
//
//  Created by Prateek Murgai on 10/9/17.
//  Copyright © 2017 Stanford University. All rights reserved.
//
/*
 Copyright (C) 2016 Apple Inc. All Rights Reserved.
 See LICENSE.txt for this sample’s licensing information
 Abstract:
 This class demonstrates the audio APIs used to capture audio data from the microphone and play it out to the speaker. It also demonstrates how to play system sounds
 */


import AudioToolbox
import AVFoundation
import CoreAudio

var audioController: AudioController = AudioController()


@objc protocol AUOutputCallbackDelegate {
    func outputCallback(_ ioActionFlags: UnsafeMutablePointer<AudioUnitRenderActionFlags>,
                        timeStamp: UnsafePointer<AudioTimeStamp>,
                        busNumber: UInt32,
                        outputNumFrames: UInt32,
                        outputDataBuffer: UnsafeMutablePointer<AudioBufferList>) -> OSStatus
}

private let AudioController_OutputCallback: AURenderCallback = {(inRefCon,
        ioActionFlags/*: UnsafeMutablePointer<AudioUnitRenderActionFlags>*/,
        inTimeStamp/*: UnsafePointer<AudioTimeStamp>*/,
        inBufNumber/*: UInt32*/,
        inNumberFrames/*: UInt32*/,
        ioData/*: UnsafeMutablePointer<AudioBufferList>*/)
    -> OSStatus
in
    let delegate = unsafeBitCast(inRefCon, to: AUOutputCallbackDelegate.self)
    let result = delegate.outputCallback(ioActionFlags,
        timeStamp: inTimeStamp,
        busNumber: inBufNumber,
        outputNumFrames: inNumberFrames,
        outputDataBuffer: ioData!)
    return result
}

@objc(AudioController)
class AudioController: NSObject, AUOutputCallbackDelegate {
    
    var procTimeStats = Stats()
    
    var ioAudioUnit: AudioUnit? = nil
    
    var auGraph : AUGraph? = nil
    
    var ioNode : AUNode = 0
    
    let sessionInstance = AVAudioSession.sharedInstance()

    var testFlag = false
    
    var mode = 0
    
    let maxDataArraySize : Int = 8192
    
    var maxFramesPerSlice : Int? = nil
    
    var micBufferList  = AudioBufferList.allocate(maximumBuffers: 1)

    var micData : [Int16]
    
    var micOn : Bool = false
    
    var speakerOn : Bool = true
    
    var preferredSpeakerSampleRate : Double = 48000
    
    var speakerSampleRate : Double = 48000
    
    var preferredSpeakerNumSamples : Int = 512
    
    var speakerNumSamples : Int {
        get {
            return Int(round(bufferDuration * speakerSampleRate))
        }
    }
    
    var preferredBufferDuration : TimeInterval {
        get {
            // Round slightly up so that audio controller selects desired block size
            return TimeInterval(Double(preferredSpeakerNumSamples + 1) / preferredSpeakerSampleRate)
        }
    }
    
    var bufferDuration : TimeInterval = 0
    
    var ioFormatInput: AudioStreamBasicDescription = AudioStreamBasicDescription()
    
    var ioFormatOutput: AudioStreamBasicDescription = AudioStreamBasicDescription()
    
    private(set) var audioChainIsBeingReconstructed: Bool = false
    
    var outputCallbackCounter : UInt32 = 0
    
    // File stuff
    var endOfFileFlag: Bool = false
    
    var audioFileFormatOutput: AudioStreamBasicDescription = AudioStreamBasicDescription()
    
    var audioFileURL: URL? = nil
    
    var audioFileObject : AVAudioFile? = nil

    var audioFileBuffer : AVAudioPCMBuffer? = nil

    var audioFileData : [Int16]? = nil
    
    var audioFileUpSampleFactor = 1  // Audio file upsample factor
    
    var audioFileDownSampleFactor = 1  // Audio file downsample factor
    
    
    func setupNotifications() {
        let notificationCenter = NotificationCenter.default
        notificationCenter.addObserver(self,
                                       selector: #selector(handleInterruption),
                                       name: AVAudioSession.interruptionNotification,
                                       object: nil)
    }
    
    @objc func handleInterruption(notification: Notification) {
        guard let userInfo = notification.userInfo,
            let typeValue = userInfo[AVAudioSessionInterruptionTypeKey] as? UInt,
            let type = AVAudioSession.InterruptionType(rawValue: typeValue) else {
                return
        }
        if type == .began {
            // Interruption began, take appropriate actions
            // startSwitch(MainViewController).setOn(false, animated: true)
        }
        else if type == .ended {
            if let optionsValue = userInfo[AVAudioSessionInterruptionOptionKey] as? UInt {
                let options = AVAudioSession.InterruptionOptions(rawValue: optionsValue)
                if options.contains(.shouldResume) {
                    // Interruption Ended - playback should resume
                } else {
                    // Interruption Ended - playback should NOT resume
                }
            }
        }
    }
    
    // Output callback function
    func outputCallback(
        _ ioActionFlags: UnsafeMutablePointer<AudioUnitRenderActionFlags>,
        timeStamp: UnsafePointer<AudioTimeStamp>,
        busNumber: UInt32,
        outputNumFrames: UInt32,
        outputDataBuffer: UnsafeMutablePointer<AudioBufferList>) -> OSStatus
    {
        let ioPtr = UnsafeMutableAudioBufferListPointer(outputDataBuffer)
        var err: OSStatus = noErr
        var audioFileNumSamples : Int = 0
        var audioFileNumSamplesRead : UInt32 = 0
        
        outputCallbackCounter += 1
        
        if !audioChainIsBeingReconstructed {
            
            // Call AudioUnitRender on the input bus of AURemoteIO
            err = AudioUnitRender(ioAudioUnit!, ioActionFlags, timeStamp, 1,
                                  UInt32(outputNumFrames), micBufferList.unsafeMutablePointer)
            
            // File processing
            if !endOfFileFlag {

                // Number of samples to obtain from audio file interface
                audioFileNumSamples = Int(AudioProcessingNumFileSamplesNeededForWrapper().numFileSamplesNeeded(for: Int32(Int(outputNumFrames))))
                
                // Read block of data from file
                audioFileNumSamplesRead = UInt32(audioFileNumSamples)
                do {
                    try audioFileObject?.read(into: audioFileBuffer!, frameCount: audioFileNumSamplesRead)
                } catch let e as CAXException {
                    NSLog("Error returned from setupAudioFile: %d: %@", e.mError, e.mOperation)
                } catch _ {
                    NSLog("Unknown error returned from audio file read opearation")
                }
                audioFileNumSamplesRead = (audioFileBuffer?.frameLength)!
                
                audioFileData = Array(UnsafeBufferPointer(start: audioFileBuffer?.int16ChannelData?[0], count: Int(maxDataArraySize)))
                
                // Fill remainder of buffer with zeros at end of file
                if Int(audioFileNumSamplesRead) < Int(audioFileNumSamples) {
                    for n in Int(audioFileNumSamplesRead) ..< Int(audioFileNumSamples) {
                        audioFileData![n] = 0
                    }
                    endOfFileFlag = true
                    print("End of file!!!!")
                }
            } else {
                audioFileNumSamples = 0
                for n in 0 ..< Int(maxDataArraySize) {
                    audioFileData![n] = 0
                }
            }

            // Get pointer to output buffer
            let outputBuffer : AudioBuffer = ioPtr[0]
            let outputBufferPointer = UnsafeMutableRawPointer(outputBuffer.mData!)
            let outputData = outputBufferPointer.assumingMemoryBound(to: Int16.self)
            
            // If mic disabled, clear mic data
            if micOn == false {
                for n in 0 ..< Int(outputNumFrames) {
                    micData[n] = 0
                }
            }
            
            // Audio processing time (tic)
            let tic0 = CFAbsoluteTimeGetCurrent()
            
            // Get pointer to mic buffer
            micData.withUnsafeMutableBufferPointer { micInputData in
                // AudioProcessing
                AudioProcessingWrapper().processAudio(
                    outputData,
                    numOutputFrames : Int32(outputNumFrames),
                    inputData :      micInputData.baseAddress,
                    micOn :          micOn,
                    fileData :       &audioFileData![0],
                    numFileFrames :  Int32(audioFileNumSamples),
                    test:            testFlag,
                    mode:            Int32(mode));
            }
            
            // Audio processing time (toc)
            let toc0 = CFAbsoluteTimeGetCurrent()
            let procTime0 = toc0 - tic0
            procTimeStats.add(newValue: procTime0)
            
            // Clear test flag
            if testFlag {
                testFlag = false
            }
            
            // Zero output
            if speakerOn == false {
                for n in 0 ..< Int(outputNumFrames) {
                    outputData[n] = 0
                }
            }
        }
        
        return err;
    }
    
    
    private func setupAudioFile() {
        
        audioFileFormatOutput = CAStreamBasicDescription(
            sampleRate: speakerSampleRate * Double(audioFileDownSampleFactor) / Double(audioFileUpSampleFactor),
            numChannels: 1,
            pcmf: .int16,
            isInterleaved: false)!
        
        if audioFileURL != nil {
            do {
                try audioFileObject = AVAudioFile(forReading: audioFileURL!, commonFormat: AVAudioCommonFormat(rawValue: 3)!, interleaved: false)
                
                audioFileBuffer = AVAudioPCMBuffer(pcmFormat: AVAudioFormat(streamDescription: &audioFileFormatOutput)!, frameCapacity: UInt32(maxDataArraySize))
            } catch let e as CAXException {
                NSLog("Error returned from setupAudioFile: %d: %@", e.mError, e.mOperation)
            } catch _ {
                NSLog("Unknown error returned from setupIOUnit")
            }
            endOfFileFlag = false
        } else {
            endOfFileFlag = true
        }
    }
    
    override init() {
        micData = Array(repeating: 0, count: maxDataArraySize)
        super.init()
        maxFramesPerSlice = maxDataArraySize
        
        audioFileData = Array(repeating: 0, count: maxDataArraySize)
        
        let emptyBuffer = AudioBuffer()
        micBufferList[0] = emptyBuffer
        micBufferList[0].mNumberChannels = 1
        micBufferList[0].mDataByteSize = UInt32(maxDataArraySize * MemoryLayout<Int16>.stride)
        micData.withUnsafeMutableBufferPointer { micData in
            micBufferList[0].mData = UnsafeMutableRawPointer(mutating: micData.baseAddress)
        }

        outputCallbackCounter = 0
    }
    
    func clearBuffers() {
        for n in 0 ..< Int(maxDataArraySize) {
            micData[n] = 0
            audioFileData![n] = 0
        }
    }
    
    private func setupAudioSession() {
        do {
            // Configure the audio session
                        
            // set the AVAudioSession to play and record category with measurement mode and to allow bluetooth
            do {
                try sessionInstance.setCategory(AVAudioSession.Category.playAndRecord, mode: AVAudioSession.Mode.measurement, options: AVAudioSession.CategoryOptions.allowBluetooth)
            } catch let error as NSError {
                try XExceptionIfError(error, "couldn't set session's allowBluetooth")
            } catch {
                fatalError()
            }

            // set the session's sample rate
            do {
                try sessionInstance.setPreferredSampleRate(preferredSpeakerSampleRate)
            } catch let error as NSError {
                try XExceptionIfError(error, "couldn't set session's preferred sample rate")
            } catch {
                fatalError()
            }
            
            // set the prefered buffer duration
            do {
                try sessionInstance.setPreferredIOBufferDuration(preferredBufferDuration)
            } catch let error as NSError {
                try XExceptionIfError(error, "couldn't set session's I/O buffer duration")
            } catch {
                fatalError()
            }
            
            // activate the audio session
            do {
                try sessionInstance.setActive(true)
            } catch let error as NSError {
                try XExceptionIfError(error, "couldn't set session active")
            } catch {
                fatalError()
            }
            
            bufferDuration = sessionInstance.ioBufferDuration
            speakerSampleRate = sessionInstance.sampleRate
                        
            // Query the audio session's ioBufferDuration and sampleRate properties
            // to determine if the preferred values were set
            print(sessionInstance)
            print("Audio Session")
            print("  ioBufferDuration: \(sessionInstance.ioBufferDuration), sampleRate: \(sessionInstance.sampleRate)")
            print("  speakerNumSamples: \(speakerNumSamples)")
            print("  inputLatency: \(sessionInstance.inputLatency), outputLatency: \(sessionInstance.outputLatency)")
            print("  availableInputs: \(String(describing: sessionInstance.availableInputs))")
            print("  inputDataSources: \(String(describing: sessionInstance.inputDataSources))")
            print("  outputDataSources: \(String(describing: sessionInstance.outputDataSources))")
            print("  currentRoute: \(String(describing: sessionInstance.outputDataSources))")
            
        } catch let e as CAXException {
            NSLog("Error returned from setupAudioSession: %d: %@", Int32(e.mError), e.mOperation)
        } catch _ {
            NSLog("Unknown error returned from setupAudioSession")
        }
        
    }
    
    
    func deactivateAudioSession() {
        do {
            // Activate the audio session
            do {
                try sessionInstance.setActive(false)
            } catch let error as NSError {
                try XExceptionIfError(error, "couldn't set session active")
            } catch {
                fatalError()
            }
            
        } catch let e as CAXException {
            NSLog("Error returned from setupAudioSession: %d: %@", Int32(e.mError), e.mOperation)
        } catch _ {
            NSLog("Unknown error returned from deactivateAudioSession")
        }
    }
    
    
    private func setupIOUnit() {
        do {
            // Create AU Graph
            try XExceptionIfError(NewAUGraph(&auGraph), "couldn't create a new instance of AUGraph")
            
            
            // Create a new instance of AURemoteIO
            var ioAudioUnitDesc = AudioComponentDescription(
                componentType: OSType(kAudioUnitType_Output),
                componentSubType: OSType(kAudioUnitSubType_RemoteIO),
                componentManufacturer: OSType(kAudioUnitManufacturer_Apple),
                componentFlags: 0,
                componentFlagsMask: 0)
            
            try XExceptionIfError(AUGraphAddNode(auGraph!, &ioAudioUnitDesc, &ioNode),
                                  "couldn't add IO node to graph")
            
            try XExceptionIfError(AUGraphOpen(auGraph!),
                                  "couldn't open AU graph")
            
            try XExceptionIfError(AUGraphNodeInfo(auGraph!, ioNode, nil, &ioAudioUnit),
                                  "couldn't get iAudioUnit")
            
            
            //  Enable input and output on AURemoteIO
            //  Input is enabled on the input scope of the input element
            //  Output is enabled on the output scope of the output element
            
            var one: UInt32 = 1
            try XExceptionIfError(AudioUnitSetProperty(self.ioAudioUnit!, AudioUnitPropertyID(kAudioOutputUnitProperty_EnableIO), AudioUnitScope(kAudioUnitScope_Input), 1, &one, SizeOf32(one)), "could not enable input on AURemoteIO")
            try XExceptionIfError(AudioUnitSetProperty(self.ioAudioUnit!, AudioUnitPropertyID(kAudioOutputUnitProperty_EnableIO), AudioUnitScope(kAudioUnitScope_Output), 0, &one, SizeOf32(one)), "could not enable output on AURemoteIO")

            
            // Explicitly set the input and output client formats
            // sample rate = speakerSampleRate, num channels = 1
            
            ioFormatInput = CAStreamBasicDescription(
                sampleRate: speakerSampleRate,
                numChannels: 1,
                pcmf: .int16,
                isInterleaved: false)!
            
            ioFormatOutput = CAStreamBasicDescription(
                sampleRate: speakerSampleRate,
                numChannels: 1,
                pcmf: .int16,
                isInterleaved: false)!
            
            try XExceptionIfError(AudioUnitSetProperty(self.ioAudioUnit!, AudioUnitPropertyID(kAudioUnitProperty_StreamFormat), AudioUnitScope(kAudioUnitScope_Output), 1, &ioFormatInput, SizeOf32(ioFormatInput)), "couldn't set the output client format on AURemoteIO")
            
            try XExceptionIfError(AudioUnitSetProperty(self.ioAudioUnit!, AudioUnitPropertyID(kAudioUnitProperty_StreamFormat), AudioUnitScope(kAudioUnitScope_Input), 0, &ioFormatOutput, SizeOf32(ioFormatOutput)), "couldn't set the input client format on AURemoteIO")

            // Read AU settings back
            var size: UInt32 = 0
            var outWritable: DarwinBoolean = false
            
            try XExceptionIfError(AudioUnitGetPropertyInfo(self.ioAudioUnit!, AudioUnitPropertyID(kAudioUnitProperty_StreamFormat), AudioUnitScope(kAudioUnitScope_Output), 1, &size, &outWritable), "couldn't get the output client format info on AURemoteIO")
            
            try XExceptionIfError(AudioUnitGetProperty(self.ioAudioUnit!, AudioUnitPropertyID(kAudioUnitProperty_StreamFormat), AudioUnitScope(kAudioUnitScope_Output), 1, &ioFormatInput, &size), "couldn't get the output client format on AURemoteIO")
            
            print("ioFormatInput \(ioFormatInput)")
            
            try XExceptionIfError(AudioUnitGetPropertyInfo(self.ioAudioUnit!, AudioUnitPropertyID(kAudioUnitProperty_StreamFormat), AudioUnitScope(kAudioUnitScope_Input), 0, &size, &outWritable), "couldn't get the output client format info on AURemoteIO")
            
            try XExceptionIfError(AudioUnitGetProperty(self.ioAudioUnit!, AudioUnitPropertyID(kAudioUnitProperty_StreamFormat), AudioUnitScope(kAudioUnitScope_Input), 0, &ioFormatOutput, &size), "couldn't get the output client format on AURemoteIO")
            
            print("ioFormatOutput \(ioFormatOutput)")
            
            // Set the MaximumFramesPerSlice property. This property is used to describe to an audio unit the maximum number
            // of samples it will be asked to produce on any single given call to AudioUnitRender
            try XExceptionIfError(AudioUnitSetProperty(self.ioAudioUnit!, AudioUnitPropertyID(kAudioUnitProperty_MaximumFramesPerSlice), AudioUnitScope(kAudioUnitScope_Global), 0, &maxFramesPerSlice, SizeOf32(UInt32.self)), "couldn't set max frames per slice on AURemoteIO")
            
            // Get the property value back from AURemoteIO. We are going to use this value to allocate buffers accordingly
            var propSize = SizeOf32(UInt32.self)
            try XExceptionIfError(AudioUnitGetProperty(self.ioAudioUnit!, kAudioUnitProperty_MaximumFramesPerSlice, kAudioUnitScope_Global, 0, &maxFramesPerSlice, &propSize), "couldn't get max frames per slice on AURemoteIO")
            
            print("maxFramesPerSlice: \(String(describing: maxFramesPerSlice))")
            
            // Set the output callback on AURemoteIO
            var outputCallbackStruct = AURenderCallbackStruct(
                inputProc: AudioController_OutputCallback,
                inputProcRefCon: Unmanaged.passUnretained(self).toOpaque()
            )
            
            try XExceptionIfError(AUGraphSetNodeInputCallback(self.auGraph!, ioNode, 0, &outputCallbackStruct),
                                  "couldn't set output element callback")
            
            var graphUpdated : DarwinBoolean = false;
            try XExceptionIfError(AUGraphUpdate(self.auGraph!, &graphUpdated),
                                  "couldn't update AU Graph")
            
            // Initialize the AURemoteIO instance
            try XExceptionIfError(AudioUnitInitialize(self.ioAudioUnit!), "couldn't initialize AURemoteIO instance")
        } catch let e as CAXException {
            NSLog("Error returned from setupIOUnit: %d: %@", e.mError, e.mOperation)
        } catch _ {
            NSLog("Unknown error returned from setupIOUnit")
        }
        
    }
    
    
    func setupAudioChain() {
        
        do {
            // Uninitialize Audio chain
            setupAudioSession()
            setupIOUnit()
            
            try XExceptionIfError(AUGraphInitialize(self.auGraph!),
                                  "couldn't initialize AU graph")
            
            setupAudioFile()

            outputCallbackCounter = 0
            
        } catch let e as CAXException {
            NSLog("Error returned from setupAudioChain: %d: %@", e.mError, e.mOperation)
        } catch _ {
            NSLog("Unknown error returned from setupAudioChain")
        }
    }
    
    @discardableResult
    func startIOUnit() -> OSStatus {
        let err = AUGraphStart(self.auGraph!)
        if err != 0 {NSLog("couldn't start AU Graph: %d", Int32(err))}
        
        procTimeStats.reset()
        
        return err
    }
    
    @discardableResult
    func stopIOUnit() -> OSStatus {
        let err = AUGraphStop(self.auGraph!)
        if err != 0 {NSLog("couldn't stop AURemoteIO: %d", Int32(err))}
        
        print("AudioProcessing::processAudio - Run time statistics")
        procTimeStats.print()
        
        return err
    }
}
