//
//  LoggingUtilities.cpp
//  EE264_lab
//
//  Created by Fernando Mujica on 12/29/22.
//  Copyright © 2022 Fernando Mujica. All rights reserved.
//

#include "LoggingUtilities.hpp"


std::ofstream makeLogFile(std::string name) {
    auto path = std::stringstream {};
    path << getenv("HOME") << "/tmp/" << name;
    return std::ofstream {path.str()};
}

