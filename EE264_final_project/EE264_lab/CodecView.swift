//
//  SwiftUIView.swift
//  EE264_lab
//
//  Created by Fernando Mujica on 1/16/23.
//

import SwiftUI

struct CodecView: View {
    @State private var disableParamEntry = false
    
    @State private var mask2NoiseRatio: Float = 35.0
    @State private var bandSelect: Int = 0
    @State private var treeDepth: Int = 1
        
    var body: some View {
        VStack {
            Image("Stanford_logo")
                .resizable()
                .aspectRatio(contentMode: .fit)
            
            Text("EE 264: Digital Signal Processing")
                .font(.title2)
            
            Text("Audio Codec Parameters")
                .font(.title3)
            
            Spacer()
            
            HStack() {
                Spacer()
                
                VStack() {
                    Text("mask2NoiseRatio (float):").frame(width: 220, height: 20, alignment: .trailing)
                    Text("bandSelect (int):").frame(width: 220, height: 20, alignment: .trailing)
                    Text("treeDepth (int):").frame(width: 220, height: 20, alignment: .trailing)
                }.multilineTextAlignment(.trailing)
                
                VStack() {
                    TextField(String(mask2NoiseRatio), value: $mask2NoiseRatio, format:.number).frame(width: 100,height: 20).keyboardType(.numberPad).disabled(disableParamEntry)
                    TextField(String(bandSelect), value: $bandSelect, format:.number).frame(width: 100,height: 20).keyboardType(.numberPad).disabled(disableParamEntry)
                    TextField(String(treeDepth), value: $treeDepth, format:.number).frame(width: 100,height: 20).keyboardType(.numberPad).disabled(disableParamEntry)
                }
                
                Spacer()
            }
            Spacer()
            Button("Setup codec parameters") {
                setupCodec()
            }
            Spacer()
        }
    }
    
    func setupCodec() -> Void {
        print("Setup Codec Parameters button pressed!")
        print("mask2NoiseRatio = \(mask2NoiseRatio)")
        print("bandSelect      = \(bandSelect)")
        print("treeDepth       = \(treeDepth)")
        
        // Pass parameters to C++
        AudioProcessingSetupCodecWrapper().setupCodec(mask2NoiseRatio, param2: Int32(bandSelect), param3: Int32(treeDepth))
    }
}


struct CodecView_Previews: PreviewProvider {
    static var previews: some View {
        CodecView()
    }
}

let codecView = CodecView()
