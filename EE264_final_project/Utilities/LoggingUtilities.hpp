//
//  LoggingUtilities.hpp
//  EE264_lab
//
//  Created by Fernando Mujica on 12/29/22.
//  Copyright © 2022 Fernando Mujica. All rights reserved.
//

#ifndef LoggingUtilities_hpp
#define LoggingUtilities_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

// makeLogFile
/*  Usage:
        #include "LoggingUtilities.hpp"
        ...
        auto out = makeLogFile("fruit.txt");
        out << "Hello, world!\n";
        out.close();
    If running the command line tool target, the file exists in the user's home directory in a /tmp sub-directory.
    the tmp directory must exist already.
    If running the App on an iPhone/iPad device, the file exists in the device's own file system, inside the app's container.
    To download the container to your computer so you can explore the files:
    - Open the "Devices" window in Xcode (keyboard shortcut ⌘⇧2)
    - Select your connected device
    - Select the EE264_lab app
    - Click the gear icon, and select "Download container"
    - In Finder, right-click/control-click the downloaded bundle and select "Show package contents"
    - Navigate to the AppData/tmp folder. The log files should be in there.
 
    Credit: Toby Bell <trfbell@stanfrod.edu>
*/
std::ofstream makeLogFile(std::string name);

#endif /* LoggingUtilities_hpp */
