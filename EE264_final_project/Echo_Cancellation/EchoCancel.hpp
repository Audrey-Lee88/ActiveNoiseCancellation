//
//  EchoCancel.hpp
//  EE264_lab
//
//  Created by EE Labs User on 3/9/23.
//

#ifndef EchoCancel_hpp
#define EchoCancel_hpp

#include <stdio.h>
#include <stdio.h>
#include <math.h>
#include "FftModule.hpp"

#include <Accelerate/Accelerate.h>

class EchoCancel {
    
    static const int maxDataArraySize = 8192;
    static const int maxFftSize = 2048;
    FftModule fftModule = FftModule(maxFftSize);

    // Temporary arrays
    float inputFloat[maxDataArraySize] = {0};
    float inputFloat3[3*maxDataArraySize] = {0};
    float outputFloat[maxDataArraySize] = {0};
    float correlationResult[2*maxDataArraySize+1] = {0};
    
    float echo_filt[2*maxDataArraySize+1] = {0};
    float filterFloat[2*maxDataArraySize+1] = {0};
    float realFloat1[2*maxDataArraySize+1 / 2] = {0};
    float imagFloat1[2*maxDataArraySize+1 / 2] = {0};
    DSPSplitComplex splitCplxFloat1 = {realFloat1, imagFloat1};
    
    float realFloat2[maxDataArraySize / 2] = {0};
    float imagFloat2[maxDataArraySize / 2] = {0};
    DSPSplitComplex splitCplxFloat2 = {realFloat2, imagFloat2};
    
    float realFilterFloat[maxDataArraySize / 2] = {0};
    float imagFilterFloat[maxDataArraySize / 2] = {0};
    DSPSplitComplex splitCplxFilterFloat = {realFilterFloat, imagFilterFloat};
    
    
public:
    
    void setup(void);

    void filter(int16_t *outputData, const int16_t *inputData,
              int inputNumSamples);
    
};

#endif /* EchoCancel_hpp */
