//
//  FreqDomainProcessing.cpp
//
//  Created by Fernando Mujica on 1/27/18.
//  Copyright © 2018 Stanford University. All rights reserved.
//

#include "FreqDomainProcessing.hpp"

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;


void FreqDomainProcessing::setup(void) {
    
    std::cout << "FreqDomainProcessing::setup\n";
    
}


void FreqDomainProcessing::test(const int16_t *inputData,
                                int inputNumSamples,
                                int mode) {
    
    cout << "FreqDomainProcessing::test\n";
    
    int log2NumInputSamples = (int)log2((double)inputNumSamples);
    
    cout << "numInputSamples = " << inputNumSamples << "\n";
    cout << "log2NumInputSamples = " << log2NumInputSamples << "\n";
    
    // Convert input integer array to float
    vDSP_vflt16(inputData, 1, inputFloat, 1, inputNumSamples);
    
    // Comment the line below (and cout.close() statement below) to send output to console
    ofstream cout("/Users/eelabsuser/Desktop/ee264_lab_4_ios/FreqDomainProcessingTest_data.txt");

    cout << "mode: \n" << mode << "\n";
    
    const float scale = 1.0 / (1 << 15);
    
    switch(mode)
    {
        case 0:
        case 1:
            cout << "Complex to complex FFT\n";
            
            // Interleave complex to split complex
            vDSP_ctoz((DSPComplex*)inputFloat, 2, &splitCplxFloat1, 1, inputNumSamples / 2);
            
            // Complex to complex FFT
            fftModule.fftNoScale(&splitCplxFloat1, log2NumInputSamples - 1, &splitCplxFloat1);
            
            for (int k = 0; k < inputNumSamples / 2; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << splitCplxFloat1.realp[k] << "\t" << setw(12) << splitCplxFloat1.imagp[k] << "\n";
            }
            for (int k = inputNumSamples / 2; k < inputNumSamples; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << 0 << "\t" << setw(12) << 0 << "\n";
            }
            
            break;
            
        case 2:
            cout << "Complex to complex IFFT (no scale)\n";
            
            // Interleave complex to split complex
            vDSP_ctoz((DSPComplex*)inputFloat, 2, &splitCplxFloat1, 1, inputNumSamples / 2);
            
            // Complex to complex FFT
            fftModule.iFftNoScale(&splitCplxFloat1, log2NumInputSamples - 1, &splitCplxFloat1);
            
            for (int k = 0; k < inputNumSamples / 2; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << splitCplxFloat1.realp[k] << "\t" << setw(12) << splitCplxFloat1.imagp[k] << "\n";
            }
            for (int k = inputNumSamples / 2; k < inputNumSamples; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << 0 << "\t" << setw(12) << 0 << "\n";
            }

            break;
            
        case 3:
            cout << "Complex to complex IFFT\n";
            
            // Interleave complex to split complex
            vDSP_ctoz((DSPComplex*)inputFloat, 2, &splitCplxFloat1, 1, inputNumSamples / 2);
            
            // Complex to complex FFT
            fftModule.iFft(&splitCplxFloat1, log2NumInputSamples - 1, &splitCplxFloat1);
            
            for (int k = 0; k < inputNumSamples / 2; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << splitCplxFloat1.realp[k] << "\t" << setw(12) << splitCplxFloat1.imagp[k] << "\n";
            }
            for (int k = inputNumSamples / 2; k < inputNumSamples; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << 0 << "\t" << setw(12) << 0 << "\n";
            }

            break;
            
            break;
            
        case 4:
            cout << "Real to complex FFT (no scale)\n";
            
            // Forward real to complex FFT
            fftModule.rFftNoScale(inputFloat, log2NumInputSamples, &splitCplxFloat1);
            
            for (int k = 0; k < inputNumSamples/2; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                    << setw(12) << splitCplxFloat1.realp[k] << "\t" << setw(12) << splitCplxFloat1.imagp[k] << "\n";
            }
            for (int k = inputNumSamples/2; k < inputNumSamples; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << 0 << "\t" << setw(12) << 0 << "\n";
            }
            break;
            
        case 5:
            cout << "Real to complex FFT\n";
            
            // Forward real to complex FFT
            fftModule.rFft(inputFloat, log2NumInputSamples, &splitCplxFloat1);
            
            for (int k = 0; k < inputNumSamples / 2; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << splitCplxFloat1.realp[k] << "\t" << setw(12) <<
                    splitCplxFloat1.imagp[k] << "\n";
            }
            for (int k = inputNumSamples / 2; k < inputNumSamples; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << 0 << "\t" << setw(12) << 0 << "\n";
            }
            break;
            
        case 6:
            cout << "Complex to real IFFT (no scale)\n";
            
            vDSP_ctoz((DSPComplex*)inputFloat, 2, &splitCplxFloat1, 1, inputNumSamples / 2);
            
            // Forward real to complex FFT
            fftModule.rIFftNoScale(&splitCplxFloat1, log2NumInputSamples, outputFloat);
            
            for (int k = 0; k < inputNumSamples / 2; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << outputFloat[k] << setw(12) << "\n";
            }
            for (int k = inputNumSamples / 2; k < inputNumSamples; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << outputFloat[k] << "\t" << setw(12) << 0 << "\n";
            }
            break;
            
        case 7:
            cout << "Complex to real IFFT\n";
            
            vDSP_ctoz((DSPComplex*)inputFloat, 2, &splitCplxFloat1, 1, inputNumSamples / 2);
            
            fftModule.rIFft(&splitCplxFloat1, log2NumInputSamples, outputFloat);
            
            for (int k = 0; k < inputNumSamples / 2; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << outputFloat[k] << setw(12) << "\n";
            }
            for (int k = inputNumSamples / 2; k < inputNumSamples; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << outputFloat[k] << "\t" << setw(12) << 0 << "\n";
            }
            break;
            
        case 10:
            cout << "Complex vector multiplication\n";
            
            // Create two split complex data structures from the first and second half of the input vector
            vDSP_ctoz((DSPComplex*)&inputFloat[0],                   2, &splitCplxFloat1, 1, inputNumSamples / 4);
            vDSP_ctoz((DSPComplex*)&inputFloat[inputNumSamples / 2], 2, &splitCplxFloat2, 1, inputNumSamples / 4);
            
            vDSP_zvmul(&splitCplxFloat1, 1, &splitCplxFloat2, 1, &splitCplxFloat1, 1, inputNumSamples / 4, 1);
            
            for (int k = 0; k < inputNumSamples / 4; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << splitCplxFloat1.realp[k] << "\t" << setw(12) << splitCplxFloat1.imagp[k] << "\n";
            }
            for (int k = inputNumSamples / 4; k < inputNumSamples; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << 0 << "\t" << setw(12) << 0 << "\n";
            }
            break;
            
        case 20:
            cout << "Circular convolution\n";
        
            fftModule.rFft(&inputFloat[0], log2NumInputSamples-1, &splitCplxFloat1);
            fftModule.rFft(&inputFloat[inputNumSamples/2], log2NumInputSamples-1, &splitCplxFloat2);
            
            vDSP_zvmul(&splitCplxFloat1, 1, &splitCplxFloat2, 1, &splitCplxFloat1, 1, inputNumSamples, 1);
            
            fftModule.rIFft(&splitCplxFloat1, log2NumInputSamples-1, outputFloat);
            
            for (int k = 0; k < inputNumSamples / 2; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << outputFloat[k] << "\t" << setw(12) << "\n";
            }
            for (int k = inputNumSamples / 2; k < inputNumSamples; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << 0 << "\t" << setw(12) << 0 << "\n";
            }
            
            break;
            
        case 30:
            cout << "Create window\n";
            
            vDSP_blkman_window(outputFloat, inputNumSamples, 0);
            // vDSP_hann_window(outputFloat, inputNumSamples, 0);
            // vDSP_hamm_window(outputFloat, inputNumSamples, 0);
            
            for (int k = 0; k < inputNumSamples; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << setw(12) << "\t" << outputFloat[k] << "\n";
            }
            break;
            
        case 32:
            cout << "Input vector times window\n";
            
            vDSP_blkman_window(outputFloat, inputNumSamples, 0);
            
            for(int i = 0; i < inputNumSamples; i++){
                outputFloat[i] = outputFloat[i]*inputFloat[i];
            }
            
            for (int k = 0; k < inputNumSamples / 2; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << outputFloat[k] << "\t" << setw(12) << "\n";
            }
            for (int k = inputNumSamples / 2; k < inputNumSamples; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << outputFloat[k] << "\t" << setw(12) << 0 << "\n";
            }
            
            break;
            
        case 33:
            cout << "rfft(x * w)\n";
            
            vDSP_blkman_window(outputFloat, inputNumSamples, 0);
            
            for(int i = 0; i < inputNumSamples; i++){
                outputFloat[i] = outputFloat[i]*inputFloat[i];
            }
            
            // Forward real to complex FFT
            fftModule.rFft(outputFloat, log2NumInputSamples, &splitCplxFloat1);
            
            for (int k = 0; k < inputNumSamples / 2; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << splitCplxFloat1.realp[k] << "\t" << setw(12) <<
                    splitCplxFloat1.imagp[k] << "\n";
            }
            for (int k = inputNumSamples / 2; k < inputNumSamples; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << 0 << "\t" << setw(12) << 0 << "\n";
            }
            break;
            
        case 40:
            cout << "Clear filter state\n";
            
            for(int i = 0; i < maxDataArraySize; i++){
                filterState[i] = 0;
            }
            
            for (int k = 0; k < maxDataArraySize; k++) {
                cout << setw(4) << "\t"
                << setw(12) << filterState[k] << "\t" << setw(12) << "\n";
            }
           
            break;
            
        case 41:
            cout << "rfft(h)\n";
            
            vDSP_vflt16(LowPassFilter, 1, filterFloat, 1, LowPassFilterLength);
            
            fftModule.rFft(filterFloat, log2NumInputSamples+1, &splitCplxFilterFloat);
            
            for (int k = 0; k < inputNumSamples / 2; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << splitCplxFilterFloat.realp[k] << "\t" << setw(12) <<
                splitCplxFilterFloat.imagp[k] << "\n";
            }
            for (int k = inputNumSamples / 2; k < inputNumSamples; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << "\t"
                << setw(12) << 0 << "\t" << setw(12) << 0 << "\n";
            }
            
            break;
            
        case 42:
            cout << "Filtering in the frequency domain with overlap and add (OLA)\n";
            
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
            
            for (int k = 0; k < inputNumSamples / 2; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputData[k] << "\t"
                << setw(12) << outputInt[k] << setw(12) << "\n";
            }
            for (int k = inputNumSamples / 2; k < inputNumSamples; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputData[k] << "\t"
                << setw(12) << outputInt[k] << "\t" << setw(12) << 0 << "\n";
            }
            
            break;
            
        case 45:
            cout << "Real to complex FFT followed by complex to real IFFT\n";
            
            // Forward real to complex FFT
            fftModule.rFft(inputFloat, log2NumInputSamples, &splitCplxFloat1);
            
            // Inverse complex to real FFT
            fftModule.rIFft(&splitCplxFloat1, log2NumInputSamples, outputFloat);
            
            for (int k = 0; k < inputNumSamples; k++) {
                cout << setw(4) << k << "\t" << setw(12) << inputFloat[k] << setw(12) << "\t" << outputFloat[k] << "\n";
            }
            break;
            
        default:
            cout << "Invalid mode\n";
            break;
    }
    
    // Comment the line below (and "ofstream cout" statement above) to send output to console
    cout.close();
}
