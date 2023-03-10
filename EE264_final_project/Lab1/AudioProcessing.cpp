//
//  AudioProcessing.cpp
//  EE264_lab
//
//  Created by Fernando Mujica on 1/27/19.
//  Copyright © 2019 Fernando Mujica. All rights reserved.
//

#include "AudioProcessing.hpp"
#include "LoggingUtilities.hpp"

using namespace std;

void AudioProcessing::setup(int outputSamplingRateParam,
                            int micNumSamplesParam,
                            int fileUpSampleFactorParam,
                            int fileDownSampleFactorParam,
                            const char* inputFileNameParam) {
    
    std::cout << "AudioProcessing::setup\n";
    std::cout << "outputSamplingRateParam   = " << outputSamplingRateParam << "\n";
    std::cout << "micNumSamplesParam        = " << micNumSamplesParam << "\n";
    std::cout << "fileUpSampleFactorParam   = " << fileUpSampleFactorParam << "\n";
    std::cout << "fileDownSampleFactorParam = " << fileDownSampleFactorParam << "\n";
    std::cout << "inputFileNameParam        = " << inputFileNameParam << "\n";
    
    callCount = 0;
    
    outputSamplingRate = outputSamplingRateParam;
    micNumSamples = micNumSamplesParam;
    fileUpSampleFactor = fileUpSampleFactorParam;
    fileDownSampleFactor = fileDownSampleFactorParam;
    strcpy(inputFileName, inputFileNameParam);
    
    
    fileNumExtraSamples = 0;
    
    // Clear internal buffers
    for(int n = 0; n < maxDataArraySize; n++) {
        tempData[n] = 0;
    }
    for(int n = 0; n < maxFileExtraSamplesSize; n++) {
        fileExtraSamples[n] = 0;
    }
    
    // Lab3? set the appropriate filter coefficients
    if (fileUpSampleFactor == 3) {
        fir1.setup(L3FirInterpFilterCoeff, L3FirInterpFilterLen);
    }
    else if (fileUpSampleFactor == 16) {
        fir1.setup(L16FirInterpFilterCoeff, L16FirInterpFilterLen);
    }
    else {
        fir1.setup(UnitImpulseFirFilterCoeff, UnitImpulseFirFilterLen);
    }
    
    // ---> Your code here! - Lab 4
//    freqDomainProc.setup();
    
    // ---> Your code here! - Lab 5
    olafilt.setup();
    echocancel.setup();
    
}


void AudioProcessing::setupCodec(float mask2NoiseRatio, int bandSelect, int treeDepth) {
    std::cout << "AudioProcessing::setupCodec\n";
    
    std::cout << "mask2NoiseRatio = " << mask2NoiseRatio << "\n";
    std::cout << "bandSelect      = " << bandSelect << "\n";
    std::cout << "treeDepth       = " << treeDepth << "\n";
}


void AudioProcessing::cleanup() {
    std::cout << "AudioProcessing::cleanup\n";
    
}


void AudioProcessing::processAudio(int16_t *outputData,
                                   int outputNumSamples,
                                   const int16_t *micData,
                                   bool micOn,
                                   const int16_t *fileData,
                                   int fileNumSamples,
                                   bool test,
                                   int mode) {

    // Zero output data array
    for (int n = 0; n < outputNumSamples; n++) {
        outputData[n] = 0;
    }
    
    // File data processing
    if (fileNumSamples > 0) {
        // Your code here! - Lab 1.1
        // Copy the audio file data to the output buffer (outputData)
        for (int n = 0; n < fileNumSamples; n++) {
            outputData[n] = fileData[n];
        }
                
        // ---> Your code here! - Lab 3.1
        fir1.filter(&tempData[fileNumExtraSamples], fileData, fileNumSamples, fileUpSampleFactor, fileDownSampleFactor);

        // Upsample and filter, store data at index fileNumExtraSamples
        fileNumExtraSamples = fileNumExtraSamples + (fileNumSamplesNeeded * fileUpSampleFactor) / fileDownSampleFactor - outputNumSamples;

        for (int i=0; i < outputNumSamples; i++) {
            outputData[i] = tempData[i];
        }
        for (int j=0; j < fileNumExtraSamples; j++) {
            fileExtraSamples[j] = tempData[j+outputNumSamples];
        }

    }
    
    // Mic data processing
    if (micOn == true) {
        // Your code here! - Lab 1.2
        // Add the mic data to the output buffer (outputData)
        // Hint: Properly scale the data to avoid overflow
        for (int n = 0; n < outputNumSamples; n++) {
            outputData[n] = (outputData[n] >> 1) + (micData[n] >> 1);
        }
    }
    
    // Output processing
    // ---> Your code here! - Lab 5
    // FIR filtering in the frequency domain via overlap add
    if (mode == 50) {
        olafilt.filter(outputData, outputData, outputNumSamples);
    }
    
    if (mode == 98) {
        echocancel.filter(outputData,outputData,outputNumSamples);
    }
    
    
    // Test button pressed
    if (test == true) {
        // Uncomment the line below (and cout.close() statement below) to send output to a file
        // std::ofstream cout("/tmp/test_data.txt");

        cout << "AudioProcessing::Test button pressed!\n";
        cout << "Mode = " << mode << "\n";
        
        // Lab 4
        freqDomainProc.test(outputData, outputNumSamples, mode);
                
        // Uncomment the line below (and "ofstream cout" statement above) to send output to a file
        // cout.close();
    }
    
    // Debug - Lab 1.3
    /*
        if ((callCount % 100) == 0)  {
            for (int n = 0; n < outputNumSamples; n++) {
                std::cout << callCount << "\t" << n << "\t" << outputData[n] << "\n";
            }
        }
    */
    
    callCount += 1; // count how many times the processAudio function has been called

}


int AudioProcessing::fileNumSamplesNeededFor(int outputNumSamples) {

    // Lab 1 - starter code
    fileNumSamplesNeeded = 0;
//    if (((outputNumSamples * fileDownSampleFactor) % fileUpSampleFactor) == 0)
//        fileNumSamplesNeeded = outputNumSamples * fileDownSampleFactor / fileUpSampleFactor;

    // get filenumsamples needed
    if (outputNumSamples*fileDownSampleFactor % fileUpSampleFactor != 0) {
        int D_tilda = (outputNumSamples / fileUpSampleFactor) * fileDownSampleFactor;
        if (fileUpSampleFactor * (outputNumSamples/fileUpSampleFactor) + fileNumExtraSamples >= outputNumSamples) {
            fileNumSamplesNeeded = D_tilda;
        }
        else {
            fileNumSamplesNeeded = D_tilda + fileDownSampleFactor;
        }
    }
    else {
        fileNumSamplesNeeded =  (outputNumSamples * fileDownSampleFactor) / fileUpSampleFactor;
    }
    
    // copy extra output samples to beginning of output array
    
    // Update num of fileNumExtraSamples
    
    return(fileNumSamplesNeeded);
}
