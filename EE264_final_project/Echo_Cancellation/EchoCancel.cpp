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


void EchoCancel::setup(void) {
    
    std::cout << "EchoCancel::setup\n";
    
}

void EchoCancel::filter(int16_t *outputData, const int16_t *inputData, int inputNumSamples){
    ofstream cout("/Users/eelabsuser/Desktop/ActiveNoiseCancellation/debug.txt");
    
    cout << inputNumSamples << "\n";
    
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

    
    vDSP_ctoz((DSPComplex*)echo_filt, 2, &splitCplxFilterFloat, 1, inputNumSamples);
    
    vDSP_zvmul(&splitCplxFloat1, 1, &splitCplxFilterFloat, 1, &splitCplxFloat1, 1, 2*inputNumSamples, 1); // H*X
    
    fftModule.rIFft(&splitCplxFloat1, log2NumInputSamples+1, outputFloat); // IDFT(H*X,2N)
    
    vDSP_vfix16(outputFloat, 1, outputData, 1, 2*inputNumSamples); // convert to int16
    
    //vDSP_vfix16(correlationResult, 1, outputData, 1, 2*inputNumSamples);
    cout.close();
}
