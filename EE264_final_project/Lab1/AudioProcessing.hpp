//
//  AudioProcessing.hpp
//  EE264_lab
//
//  Created by Fernando Mujica on 1/27/19.
//  Copyright © 2019 Fernando Mujica. All rights reserved.
//

#ifndef AudioProcessing_hpp
#define AudioProcessing_hpp

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>


// Lab 4
#include "FreqDomainProcessing.hpp"
#include "OlaFftFilter.hpp"
#include "FirInterp.hpp"
// ---> Your code here! - Lab 5

// For project
#include "NoiseCancel.hpp"
#include <Accelerate/Accelerate.h>


class AudioProcessing {
    
    // Counter for the number of times the processAudio() function has been called
    int callCount = 0;
    
    int outputSamplingRate;    // Output sampling rate
    int micNumSamples;         // N
    int fileUpSampleFactor;    // L
    int fileDownSampleFactor;  // M
    char inputFileName[80];    // Input file name
    
    // Temporary array to store rate converted audio file data
    static const int maxDataArraySize = 8192;
    int16_t tempData[maxDataArraySize];
    
    int16_t tempData2[666666];
    int16_t tempData3[666666];
    
    // Number of samples to read fr om the audio file. See fileNumSamplesNeededFor().
    int fileNumSamplesNeeded = 0;
    
    // These properties are used to keep track of extra audio file samples produced
    int fileNumExtraSamples = 0;
    static const int maxFileExtraSamplesSize = 64;
    int16_t fileExtraSamples[maxFileExtraSamplesSize] = {0};
    
    // Lab 4
    FreqDomainProcessing freqDomainProc;
    FirInterp fir1;
    OlaFftFilter olafilt;
    
    //For project
    NoiseCancel noiseCancel;
    
    
public:
    
    /**
     The setup function is called once after the user interface Start switch is enabled.

     @param outputSamplingRateParam N
     @param micNumSamplesParam N
     @param fileUpSampleFactorParam L
     @param fileDownSampleFactorParam M
     @param inputFileNameParam Input file name
     */
    void setup(int outputSamplingRateParam,
               int micNumSamplesParam,
               int fileUpSampleFactorParam,
               int fileDownSampleFactorParam,
               const char* inputFileNameParam);
    
    
    /**
     The setup function is called once after the user interface Start switch is enabled.

     @param mask2NoiseRatio Parameter 1
     @param bandSelect Parameter 2
     @param treeDepth Parameter 3
     */
    void setupCodec(float mask2NoiseRatio, int bandSelect, int treeDepth);
    
    
    /**
     The cleanup function is called once after the user interface Start switch is disabled.
     */
    void cleanup();
    
    /**
     This function is called by the iOS audio interface when a block of output data
     is needed.  It provides inputs from the microphone interface (if enabled) and
     the audio file (if available).  It also passes the state of the application
     user interface elements.

     @param outputData Array to store output samples
     @param outputNumSamples Number of output samples
     @param micData Microphone input array
     @param micOn State of micOn user interface switch
     @param fileData Audio file input array
     @param fileNumSamples Number of samples in file array
     @param test State of test user interface button
     @param mode Value of mode user interface text field
     */
    void processAudio(int16_t *outputData,
                      int outputNumSamples,
                      const int16_t *micData,
                      bool micOn,
                      const int16_t *fileData,
                      int fileNumSamples,
                      bool test,
                      int mode);
    
    
    /**
     This function returns the number of samples to read from the audio file such that,
     after interpolation, the number of samples equals N, i.e.,
     N = fileNumSamplesNeeded * L / M
     Note that depending on the values of L and M and N, the above equality requires
     fileNumSamplesNeeded to be a fraction, however a fractional number of samples is only reasonable
     in the average sense, thus, the number of file samples requested must be jittered so that
     avg(fileNumSamplesNeeded) = N * M / L

     @param outputNumSamples Number of output samples needed
     @return Number of file samples needed
     */
    int fileNumSamplesNeededFor(int outputNumSamples);
    
    // ---> Your code here! - Lab 1.3
    // ---> STARTER CODE DELETE START
    /**
     Lab 1.3: Print debug data to console (Mode = 1) or /tmp/debug.txt file (Mode = 2)
     
     @param mode Value of mode user interface text field
     @param outputNumSamples Number of output samples
     @param outputData Output data array
     @param micData Microphone data array
     @param fileData File data array
     */
    void debug(int mode, int outputNumSamples, const int16_t *outputData, const int16_t *micData, const int16_t *fileData);
    // ---> STARTER CODE DELETE END
};

#endif /* AudioProcessing_hpp */
