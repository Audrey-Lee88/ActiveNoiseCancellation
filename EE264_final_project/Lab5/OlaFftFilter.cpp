//
//  OlaFftFilter.cpp
//  EE264_lab
//
//  Created by EE Labs User on 2/16/23.
//

#include "OlaFftFilter.hpp"

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;


void OlaFftFilter::setup(void) {
    
    std::cout << "OlaFftFilter::setup\n";
    
    cout << "rfft(h)\n";
    
    vDSP_vflt16(LowPassFilter, 1, filterFloat, 1, LowPassFilterLength);
    
    fftModule.rFft(filterFloat, log2OlaFftSize, &splitCplxFilterFloat);
}

void OlaFftFilter::filter(int16_t *outputData, const int16_t *inputData, int inputNumSamples){
    
    const float scale = 1.0 / (1 << 15);
    int log2NumInputSamples = (int)log2((double)inputNumSamples);
    
    vDSP_vflt16(inputData, 1, inputFloat, 1, inputNumSamples);
    
    fftModule.rFft(inputFloat, log2NumInputSamples+1, &splitCplxFloat1); // x -> X
    
    vDSP_vflt16(LowPassFilter, 1, filterFloat, 1, LowPassFilterLength); // getting h
    
    fftModule.rFft(filterFloat, log2NumInputSamples+1, &splitCplxFilterFloat); // h -> H
    
    vDSP_zvmul(&splitCplxFloat1, 1, &splitCplxFilterFloat, 1, &splitCplxFloat1, 1, 2*inputNumSamples, 1); // H*X
    
    fftModule.rIFft(&splitCplxFloat1, log2NumInputSamples+1, outputFloat); // IDFT(H*X,2N)
    
    vDSP_vsmul(outputFloat, 1, &scale, outputFloat, 1, maxDataArraySize);
    
//            for(int i =0; i < maxDataArraySize; i++){
//                outputFloat[i] = outputFloat[i]*scale;
//            }
    
    vDSP_vfix16(outputFloat, 1, outputInt, 1, 2*inputNumSamples); // convert to int16
    
    // overlap add
    for(int i = 0; i < LowPassFilterLength - 1; i++){
        outputInt[i] = outputInt[i] + filterState[i];
        filterState[i] = outputInt[i+inputNumSamples];
    }
    
}

