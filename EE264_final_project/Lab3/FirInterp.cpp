//
//  FirInterp.cpp
//  EE264_lab
//
//  Created by EE Labs User on 2/2/23.
//  Copyright © 2023 Fernando Mujica. All rights reserved.
//

#include "FirInterp.hpp"

using namespace std;

void FirInterp::setup(const int16_t *filterCoeff,
                      int inFilterLen){
    if (inFilterLen < maxFilterLength){
        filterLen = inFilterLen;
    }
    else{
        filterLen = maxFilterLength;
    }
    for (int i = 0; i < filterLen; i++){
        coeff[i] = filterCoeff[i]; // copy the filterCoeff array val to coeff array
    }
    for (int i = 0; i < maxFilterLength; i++){
        state[i] = 0; // clear filter state array
    }
}

void FirInterp::filter(int16_t *outputData,
                       const int16_t *inputData,
                       int inputNumSamples,
                       int upSampleFactor,
                       int downSampleFactor){
    int16_t tempData1[maxDataArraySize];
    
    int N = inputNumSamples*upSampleFactor;
    //if (inputNumSamples*upSampleFactor <= maxDataArraySize){
    for(int n = 0; n < inputNumSamples; n++) {
        tempData[n*upSampleFactor] = inputData[n];
        for(int k = 1; k < upSampleFactor; k++){
            tempData[n*upSampleFactor+k] = 0;
        }
    }
    
    // for 0 < n < K-1
    for (int n = 0; n < filterLen - 1; n++){
        int32_t acc = 0;
        for (int k = 0; k < n + 1; k++){
            acc += tempData[n - k] * coeff[k];
        }
        for (int k = n + 1; k < filterLen; k++){
            acc += state[k-(n+1)] * coeff[k];
        }
        tempData1[n] = acc >> 15;
        // LMS coefficient adaptation
    }
    for (int n = filterLen - 1; n < N; n++){
        int32_t acc = 0;
        for (int k =0; k < filterLen; k++){
            acc += tempData[n - k] * coeff[k];
        }
        tempData1[n] = acc >> 15;
        // LMS coefficient adaptation
    }
    
    int n = 0;
    while(downSampleFactor * n < inputNumSamples * upSampleFactor){
        outputData[n] = tempData1[downSampleFactor*n];
        n++;
    }
        

    for (int i = 0; i < filterLen - 1; i++){
        state[i] = tempData[inputNumSamples * upSampleFactor - 1 - i];
    }
}

// restrictions about implementation:
// we cannot upsample or downsample by a non-integer number. We need upsample*number of input samples < max data array
// we used the Parks-McClellan method filter because it most closely resembled our ideal/optimal filter (in the freq. domain plot in matlab)
// the quantizing did affect our freq. response bc it affected the ripples in our stop band. 

//    for (int n = 0; n < filterLen - 1; n++){
//        if (n%downSampleFactor == 0){
//            int32_t acc = 0;
//            // previous block data
//            for (int k = n + 1; k < filterLen; k++){
//                if (state[n - k + N] == 0 or coeff[k] == 0){
//                    continue;
//                }
//                else{
//                    acc += state[n - k + N] * coeff[k];
//                }
//            }
//            // curr block data
//            for (int k = 0; k < n; k++){
//                if (tempData[n - k] == 0 or coeff[k] == 0){
//                    continue;
//                }
//                else{
//                    acc += tempData[n - k] * coeff[k];
//                }
//            }
//            outputData[n/downSampleFactor] = acc >> 15;
//        }
//        else{
//            continue;
//        }
//    }
//
//    // for K-1 < n < N
//    for (int n = filterLen - 1; n < N; n++){
//        if (n%downSampleFactor == 0){
//            int32_t acc = 0;
//            for (int k =0; k < filterLen; k++){
//                if (tempData[n - k] == 0 or coeff[k] == 0){
//                    continue;
//                }
//                else{
//                    acc += tempData[n - k] * coeff[k];
//                }
//            }
//            outputData[n/downSampleFactor] = acc >> 15;
//        }
//        else{
//            continue;
//        }
//    }
