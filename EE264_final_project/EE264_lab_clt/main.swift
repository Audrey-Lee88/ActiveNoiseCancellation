//
//  main.swift
//  EE264_lab_clt
//
//  Created by Fernando Mujica on 12/3/22.
//  Copyright © 2022 Fernando Mujica. All rights reserved.
//

import Foundation
import ArgumentParser

import AudioToolbox
import AVFoundation
import CoreAudio

let maxDataArraySize : Int = 8192

var procTimeStats = Stats()


struct EE264_lab_clt: ParsableCommand {
    
    static let configuration = CommandConfiguration(
            abstract: "EE264 Lab Command Line Tool.")
    
    @Argument(help: "Mic file name")    var micFile: String
    @Argument(help: "Input file name")  var inputFile: String
    @Argument(help: "Output file name") var outputFile: String
    @Option(name: .customShort("N"), help: "N: mic and output block size")  var blockSize = 512
    @Option(name: .customShort("L"), help: "L: File Up-Sample Ratio")       var fileUpSampleRatio = 1
    @Option(name: .customShort("M"), help: "M: File Down-Sample Ratio")     var fileDownSampleRatio = 1
    @Option(help: "mode") var mode = 0
    
    // File stuff
    var inputEndOfFileFlag: Bool = false
    
    mutating func run() throws {
        print("EE264 Lab Command Line Tool")
        print("Mic file name:             ", micFile)
        print("Input file name:           ", inputFile)
        print("Output file name:          ", outputFile)
        print("Block size (N):            ", blockSize)
        print("File Up-Sample Ratio (L):  ", fileUpSampleRatio)
        print("File Down-Sample Ratio (M):", fileDownSampleRatio)
        
        // List .caf files in current directory
        let audioFileURLsArray : [URL] = Bundle.main.urls(forResourcesWithExtension: "caf", subdirectory: ".")!
        print(audioFileURLsArray)
        
        
        // Init (open mic and input files to read, open output file to write, compute derived parameters)
        
        // Create mic, input, and output audio buffers
        let audioBufferFormat = AVAudioFormat(
            commonFormat: AVAudioCommonFormat.pcmFormatInt16,
            sampleRate: 48000,
            channels: 1,
            interleaved: false)!
        
        let micFileBuffer =    AVAudioPCMBuffer(pcmFormat: audioBufferFormat, frameCapacity: UInt32(maxDataArraySize))
        let inputFileBuffer =  AVAudioPCMBuffer(pcmFormat: audioBufferFormat, frameCapacity: UInt32(maxDataArraySize))
        let outputFileBuffer = AVAudioPCMBuffer(pcmFormat: audioBufferFormat, frameCapacity: UInt32(maxDataArraySize))

        // Open mic file
        let micFileURL = URL(string: "file://" + micFile)!
        guard let micFileObject = try? AVAudioFile(forReading: micFileURL, commonFormat: AVAudioCommonFormat.pcmFormatInt16, interleaved: false) else
        {
            fatalError("Mic file not found")
        }

        // Open input file
        let inputFileURL = URL(string: "file://" + inputFile)!
        guard let inputFileObject = try? AVAudioFile(forReading: inputFileURL, commonFormat: AVAudioCommonFormat.pcmFormatInt16, interleaved: false) else
        {
            fatalError("Input file not found")
        }
        
        // Open output file
        let outputFileURL = URL(string: "file://" + outputFile)!
        let settings: [String: Any] = [
            AVFormatIDKey: outputFileBuffer!.format.settings[AVFormatIDKey] ?? kAudioFormatLinearPCM,
            AVNumberOfChannelsKey: outputFileBuffer!.format.settings[AVNumberOfChannelsKey] ?? 1,
            AVSampleRateKey: outputFileBuffer!.format.settings[AVSampleRateKey] ?? 48000,
            AVLinearPCMBitDepthKey: outputFileBuffer!.format.settings[AVLinearPCMBitDepthKey] ?? 16
        ]
        guard let outputFileObject = try? AVAudioFile(forWriting: outputFileURL, settings: settings, commonFormat: AVAudioCommonFormat.pcmFormatInt16, interleaved: false)
        else {
            fatalError("Output file not found")
        }

        // C++ setup
        AudioProcessingSetupWrapper().setup( 1,
                                             numInputFrames: Int32(blockSize),
                                             upSampleFactor: Int32(fileUpSampleRatio),
                                             downSampleFactor: Int32(fileDownSampleRatio),
                                             inputFileName: inputFile)
        
        // Run (read block of mic and input samples, run main compute function)
        while !inputEndOfFileFlag {
            
            do {
                try micFileObject.read(into: micFileBuffer!, frameCount: AVAudioFrameCount(blockSize))
            } catch {
                print("Failed to read block of mic data")
            }
            
            // Number of samples to obtain from audio file interface
            let inputFileBlockSize = Int(AudioProcessingNumFileSamplesNeededForWrapper().numFileSamplesNeeded(for: Int32(blockSize)))
            print("inputFileNumSamples: \(inputFileBlockSize)")
            
            do {
                try inputFileObject.read(into: inputFileBuffer!, frameCount: AVAudioFrameCount(inputFileBlockSize))
            } catch {
                print("Failed to read block of input data")
            }
            
            guard let micInt16ChannelData = micFileBuffer?.int16ChannelData else {
                fatalError("Failed to obtain underlying input buffer")
            }
            guard let inputInt16ChannelData = inputFileBuffer?.int16ChannelData else {
                fatalError("Failed to obtain underlying input buffer")
            }
            guard let outputInt16ChannelData = outputFileBuffer?.int16ChannelData else {
                fatalError("Failed to obtain underlying output buffer")
            }
            
            let micData: UnsafeMutablePointer<Int16> = micInt16ChannelData[0]
            let inputData: UnsafeMutablePointer<Int16> = inputInt16ChannelData[0]
            let outputData: UnsafeMutablePointer<Int16> = outputInt16ChannelData[0]
            
            let micFileNumSamplesRead = micFileBuffer?.frameLength ?? 0
            let inputFileNumSamplesRead = inputFileBuffer?.frameLength ?? 0

            // Zero pad mic buffer
            if Int(micFileNumSamplesRead) < blockSize {
                for n in Int(micFileNumSamplesRead) ..< blockSize {
                    micData[n] = 0
                }
                micFileBuffer?.frameLength = AVAudioFrameCount(blockSize)
                print("End of mic data!!!!")
            }
            
            // Zero pad input buffer
            if Int(inputFileNumSamplesRead) < inputFileBlockSize {
                for n in Int(inputFileNumSamplesRead) ..< inputFileBlockSize {
                    inputData[n] = 0
                }
                inputEndOfFileFlag = true
                inputFileBuffer?.frameLength = AVAudioFrameCount(inputFileBlockSize)
                print("End of input data!!!!")
            }

            
            // Process data
            let tic0 = CFAbsoluteTimeGetCurrent()
        
            AudioProcessingWrapper().processAudio(
                outputData,
                numOutputFrames : Int32(blockSize),
                inputData :      micData,
                micOn :          true,
                fileData :       inputData,
                numFileFrames :  Int32(inputFileBlockSize),
                test:            false,
                mode:            Int32(mode));
            
            let toc0 = CFAbsoluteTimeGetCurrent()
            let procTime0 = toc0 - tic0
            procTimeStats.add(newValue: procTime0)
            
            outputFileBuffer?.frameLength = micFileBuffer!.frameLength
            
            do {
                try outputFileObject.write(from: outputFileBuffer!)
            } catch {
                print("Failed to write block of output data")
            }
        }
        
        
        // Cleanup
        AudioProcessingCleanupWrapper().cleanup()
        
        print("AudioProcessing::processAudio - Run time statistics")
        procTimeStats.print()
        
        print("All done!")
        
    }
}

EE264_lab_clt.main()
