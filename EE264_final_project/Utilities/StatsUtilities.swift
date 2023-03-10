//
//  StatsUtilities.swift
//  EE264_lab
//
//  Created by Fernando Mujica on 12/29/22.
//  Copyright © 2022 Fernando Mujica. All rights reserved.
//

import Foundation

class Stats {
    var min:Double = 1.0
    var max:Double = 0.0
    var sum:Double = 0.0
    var cnt:Int    = 0
    var avg:Double {
        get {
            return sum / Double(cnt)
        }
    }
    
    func reset() {
        min = 1.0
        max = 0.0
        sum = 0.0
        cnt = 0
    }
    
    func add(newValue : Double) {
        cnt += 1
        sum += newValue
        min = Swift.min(min, newValue)
        max = Swift.max(max, newValue)
    }
    
    func print() {
        Swift.print("cnt = \(cnt)")
        Swift.print("min = \(min)")
        Swift.print("max = \(max)")
        Swift.print("avg = \(avg)")
    }
    
}
