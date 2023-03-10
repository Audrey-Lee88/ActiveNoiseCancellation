//
//  ContentView.swift
//  EE264_lab
//
//  Created by Fernando Mujica on 1/15/23.
//

import SwiftUI

struct ContentView: View {
    var body: some View {
            TabView {
                mainView
                codecView
            }
            .tabViewStyle(.page)
            .indexViewStyle(.page(backgroundDisplayMode: .always))
        }
}


struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
.previewInterfaceOrientation(.portrait)
    }
}
