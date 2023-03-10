//
//  FftModule.cpp
//  EE264_lab
//
//  Created by Fernando Mujica on 2/11/18.
//  Copyright © 2018 Stanford University. All rights reserved.
//

#include "FftModule.hpp"

FftModule::FftModule(int maxFftSize) {
   
    int log2MaxFftSize = (int)log2((double)maxFftSize);
    
    fftSetup = vDSP_create_fftsetup(log2MaxFftSize, kFFTRadix2);
}

FftModule::~FftModule() {
    
    vDSP_destroy_fftsetup(fftSetup);
}


void FftModule::fftNoScale(const DSPSplitComplex *input, int log2N, DSPSplitComplex *output) {
    
    // Complex to complex FFT
    vDSP_fft_zop(fftSetup, output, 1, output, 1, log2N, kFFTDirection_Forward);
}


void FftModule::fft(const DSPSplitComplex *input, int log2N, DSPSplitComplex *output) {
    
    // int N = 1 << log2N;
    
    fftNoScale(input, log2N, output);
}


void FftModule::iFftNoScale(const DSPSplitComplex *input, int log2N, DSPSplitComplex *output) {
    
    // Complex to complex FFT
    vDSP_fft_zop(fftSetup, output, 1, output, 1, log2N, kFFTDirection_Inverse);
}


void FftModule::iFft(const DSPSplitComplex *input, int log2N, DSPSplitComplex *output) {
    
    int N = 1 << log2N;
    
    iFftNoScale(input, log2N, output);
    
    // Scale
    float scale = 1.0 / N;
    vDSP_vsmul(output->realp, 1, &scale, output->realp, 1, N);
    vDSP_vsmul(output->imagp, 1, &scale, output->imagp, 1, N);
}


void FftModule::rFftNoScale(const float *input, int log2N, DSPSplitComplex *output) {
    
    int N = 1 << log2N;
    
    // Scramble-pack the real data into complex buffer in just the way that's
    // required by the real-to-complex FFT function that follows.
    vDSP_ctoz((DSPComplex*)input, 2, output, 1, N / 2);
    
    // Do real->complex forward FFT
    vDSP_fft_zrop(fftSetup, output, 1, output, 1, log2N, kFFTDirection_Forward);
}


void FftModule::rFft(const float *input, int log2N, DSPSplitComplex *output) {
    
    int N = 1 << log2N;
    
    rFftNoScale(input, log2N, output);
    
    // Scale
    float scale = 0.5;
    vDSP_vsmul(output->realp, 1, &scale, output->realp, 1, N / 2);
    vDSP_vsmul(output->imagp, 1, &scale, output->imagp, 1, N / 2);
}


void FftModule::rIFftNoScale(const DSPSplitComplex *input, int log2N, float *output) {
    
    int N = 1 << log2N;
    
    // Inverse complex to real FFT
    vDSP_fft_zrip(fftSetup, input, 1, log2N, kFFTDirection_Inverse);
    
    // This leaves result in packed format. Here we unpack it into a real vector.
    vDSP_ztoc(input, 1, (DSPComplex*)output, 2, N/2);
}


void FftModule::rIFft(const DSPSplitComplex *input, int log2N, float *output) {
    
    int N = 1 << log2N;
    
    rIFftNoScale(input, log2N, output);
    
    // Scale
    float scale = 1.0 / N;
    vDSP_vsmul(output, 1, &scale, output, 1, N);
}
