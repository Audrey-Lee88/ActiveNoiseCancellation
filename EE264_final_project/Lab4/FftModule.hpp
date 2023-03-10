//
//  FftModule.hpp
//  EE264_lab
//
//  Created by Fernando Mujica on 2/11/18.
//  Copyright © 2018 Stanford University. All rights reserved.
//

#ifndef FftModule_hpp
#define FftModule_hpp

#include <stdio.h>

#include <Accelerate/Accelerate.h>

class FftModule {
    
public:
    
    FftModule(int maxFftSize);
    
    ~FftModule();

    // Set up a data structure with pre-calculated twiddle factors for
    // computing the FFT. The structure is opaque, but is needed by all
    // Accelerate FFT functions.  One FFTSetup can be used for FFT sizes
    // up to the specified size.  Only FFT sizes power of 2 are supported.
    FFTSetup fftSetup;
    
    void fft(const DSPSplitComplex *input, int log2N, DSPSplitComplex *output);
    void fftNoScale(const DSPSplitComplex *input, int log2N, DSPSplitComplex *output);

    void iFft(const DSPSplitComplex *input, int log2N, DSPSplitComplex *output);
    void iFftNoScale(const DSPSplitComplex *input, int log2N, DSPSplitComplex *output);

    void rFft(const float *input, int log2N, DSPSplitComplex *output);
    void rFftNoScale(const float *input, int log2N, DSPSplitComplex *output);

    void rIFft(const DSPSplitComplex *input, int log2N, float *output);
    void rIFftNoScale(const DSPSplitComplex *input, int log2N, float *output);
};

#endif /* FftFunctions_hpp */
