//
//  EchoCancel.cpp
//  EE264_lab
//
//  Created by EE Labs User on 3/9/23.
//

#include "EchoCancel.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;


void EchoCancel::setup(const int16_t *inputData,
                       int inputNumSamples){
    
    std::cout << "EchoCancel::setup\n";
    std::cout << inputNumSamples;
    
    ofstream cout("/Users/eelabsuser/Desktop/ActiveNoiseCancellation/debug.txt");
    
    cout << inputNumSamples << "\n";
    
    const float scale = 1.0 / (1 << 15);
    int log2NumInputSamples = (int)log2((double)inputNumSamples);

    vDSP_vflt16(inputData, 1, inputFloat, 1, inputNumSamples);
    
    #define NF  inputNumSamples
    #define NC  (2*inputNumSamples+1)

    for (int i = 0; i < inputNumSamples; i++){
        inputFloat3[i+inputNumSamples] = inputFloat[i];
    }
    
    vDSP_conv(inputFloat3, 1, inputFloat, 1, correlationResult, 1, NC, NF);

    float max_val = 0;
    int max_val_index = 0;
    for (int i = 0; i < NC; i++){
        //cout << correlationResult[i] << "\t";
        //outputFloat[i] = correlationResult[i];
        if (correlationResult[i] > max_val){
            max_val = correlationResult[i];
            cout << max_val << "\t" << i << "\n";
            max_val_index = i;
        }

    }
    cout << NC << "\t" << max_val_index;
    echo_filt[0] = 1;
    echo_filt[max_val_index] = 0.5;
    filterLen = max_val_index + 1;
    
    cout.close();
    
}

void EchoCancel::filter(int16_t *outputData,
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
            acc += tempData[n - k] * echo_filt[k];
        }
        for (int k = n + 1; k < filterLen; k++){
            acc += state[k-(n+1)] * echo_filt[k];
        }
        tempData1[n] = acc >> 15;
        
    }
    for (int n = filterLen - 1; n < N; n++){
        int32_t acc = 0;
        for (int k =0; k < filterLen; k++){
            acc += tempData[n - k] * echo_filt[k];
        }
        tempData1[n] = acc >> 15;
    }
    
    int n = 0;
    while(downSampleFactor * n < inputNumSamples * upSampleFactor){
        outputData[n] = tempData1[downSampleFactor*n];
        n++;
    }
        

    for (int i = 0; i < inputNumSamples; i++){
        state[i+inputNumSamples] = state[i];
        state[i] = outputData[inputNumSamples * upSampleFactor - 1 - i];
    }
    
    

}



//vDSP_zvmul(&splitCplxFloat1, 1, &splitCplxFilterFloat, 1, &splitCplxFloat1, 1, 2*inputNumSamples, 1); // H*X
//
//fftModule.rIFft(&splitCplxFloat1, log2NumInputSamples+1, outputFloat); // IDFT(H*X,2N)
//
//vDSP_vsmul(outputFloat, 1, &scale, outputFloat, 1, 2*inputNumSamples);
//
//vDSP_vfix16(outputFloat, 1, outputData, 1, 2*inputNumSamples); // convert to int16
//
////vDSP_vfix16(correlationResult, 1, outputData, 1, 2*inputNumSamples);
//cout.close();
