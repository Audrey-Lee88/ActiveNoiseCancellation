//
//  SwiftUIView.swift
//  EE264_lab
//
//  Created by Fernando Mujica on 1/16/23.
//

import SwiftUI

struct MainView: View {
    @State private var disableParamEntry = false
    
    @State private var outputSamplingRate: Int = 48000
    @State private var outputBlockSize: Int = 512
    @State private var fileUpSampleRatio: Int = 1
    @State private var fileDownSampleRatio: Int = 1
    @State private var fileAvgBlockSize: Float = 512
    @State private var selectedFileIndex: Int = 0
    @State private var mode: Int = 0
    @State private var micOn = false
    @State private var speakerOn = true
    @State private var startOn = false
    
    var audioFilesArray: [String] = []
    
    let audioFileURLsArray : [URL] = Bundle.main.urls(forResourcesWithExtension: "caf", subdirectory: ".")!
    
    init() {
        print("File URLs")
        for url in audioFileURLsArray {
            print(url.lastPathComponent)
            audioFilesArray.append(url.lastPathComponent)
        }
        print(audioFilesArray)
    }
    
    var body: some View {
        VStack {
            Image("Stanford_logo")
                .resizable()
                .aspectRatio(contentMode: .fit)
            
            Text("EE 264: Digital Signal Processing")
                .font(.title2)
            
            Spacer()
            
            HStack() {
                Spacer()
                
                VStack() {
                    Text("Mic/Speaker rate fs (Hz):").frame(width: 220, height: 20, alignment: .trailing)
                    Text("Mic/Speaker block size (N):").frame(width: 220, height: 20, alignment: .trailing)
                    Text("File up-sample ratio (L):").frame(width: 220, height: 20, alignment: .trailing)
                    Text("File down-sample ratio (M):").frame(width: 220, height: 20, alignment: .trailing)
                    Text("File average block size:").frame(width: 220, height: 20, alignment: .trailing)
                }.multilineTextAlignment(.trailing)
                
                VStack() {
                    TextField(String(outputSamplingRate), value: $outputSamplingRate, format:.number).frame(width: 100,height: 20).keyboardType(.numberPad).disabled(disableParamEntry)
                    TextField(String(outputBlockSize), value: $outputBlockSize, format:.number).frame(width: 100,height: 20).keyboardType(.numberPad).disabled(disableParamEntry)
                    TextField(String(fileUpSampleRatio), value: $fileUpSampleRatio, format:.number).frame(width: 100,height: 20).keyboardType(.numberPad).disabled(disableParamEntry)
                    TextField(String(fileDownSampleRatio), value: $fileDownSampleRatio, format:.number).frame(width: 100,height: 20).keyboardType(.numberPad).disabled(disableParamEntry)
                    TextField(String(fileAvgBlockSize), value: $fileAvgBlockSize, format:.number).frame(width: 100,height: 20).keyboardType(.numberPad).disabled(disableParamEntry)
                }
                
                Spacer()
            }
            
            Picker(selection: $selectedFileIndex, label: Text("Select a File")) {
                ForEach(0 ..< audioFilesArray.count, id: \.self) {
                    Text(audioFilesArray[$0])
                }
            }
            .pickerStyle(.wheel)
            .disabled(disableParamEntry)
            
            Spacer()
            
            HStack {
                Spacer()
                
                Button("Test") {
                    testCallback()
                }
                
                Text("Mode")
                
                TextField("0", value: $mode, formatter: NumberFormatter())
                    .frame(width: 40.0)
                
                Spacer()
                
                Toggle("Mic On", isOn: $micOn)
                    .frame(width: 150.0).disabled(disableParamEntry)
                
                Spacer()
                
            }
            
            HStack {
                Spacer()
                
                Toggle("Start", isOn: $startOn)
                    .onChange(of: startOn) { value in
                        //perform your action here...
                        if value == true {
                            print("Start is On. Selected file index = \(selectedFileIndex)")
                            disableParamEntry = true
                            startAudio()
                        }
                        else {
                            print("Start is Off")
                            disableParamEntry = false
                            stopAudio()
                        }
                    }
                    .frame(width: 120.0)
                    
                Spacer()
                
                Toggle("Speaker On", isOn: $speakerOn)
                    .frame(width: 150.0).disabled(disableParamEntry)
                
                Spacer()
                
            }

            Spacer()
            
            Image("ee264_lab")
                .resizable()
                .aspectRatio(contentMode: .fit)
                .padding()
        }
    }
    
    func startAudio() -> Void {
        // Clear buffers
        audioController.clearBuffers()
        
        // Set mode
        audioController.mode = Int(mode)
        
        // Update parameters from UI
        audioController.preferredSpeakerSampleRate = Double(outputSamplingRate)
        audioController.preferredSpeakerNumSamples = Int(outputBlockSize)
        audioController.audioFileUpSampleFactor = Int(fileUpSampleRatio)
        audioController.audioFileDownSampleFactor = Int(fileDownSampleRatio)
        audioController.audioFileURL = audioFileURLsArray[selectedFileIndex]
        audioController.micOn = micOn
        audioController.speakerOn = speakerOn
        
        // Call C++ setup funtion
        AudioProcessingSetupWrapper().setup(
            Int32(audioController.preferredSpeakerSampleRate),
            numInputFrames: Int32(audioController.preferredSpeakerNumSamples),
            upSampleFactor: Int32(audioController.audioFileUpSampleFactor),
            downSampleFactor: Int32(audioController.audioFileDownSampleFactor),
            inputFileName: audioFilesArray[selectedFileIndex].cString(using: String.Encoding.utf8)
        )
        
        // Enable sound
        audioController.setupAudioChain()
        audioController.startIOUnit()
    }
    
    func stopAudio() -> Void {
        // Disable sound
        audioController.stopIOUnit()
        audioController.deactivateAudioSession()

        // Call C++ cleanup funtion
        AudioProcessingCleanupWrapper().cleanup()
    }
    
    func testCallback() -> Void {
        print("Test button pressed!")
        print("Mode: \(mode)")
        audioController.testFlag = true
        audioController.mode = Int(mode)
    }

}

struct MainView_Previews: PreviewProvider {
    static var previews: some View {
        MainView()
    }
}

let mainView = MainView()
