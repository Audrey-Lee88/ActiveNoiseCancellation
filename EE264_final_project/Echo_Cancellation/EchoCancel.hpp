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
    //    static const int maxFftSize = 2048;
    //    FftModule fftModule = FftModule(maxFftSize);
    //    static const int maxFilterLength = 2*maxDataArraySize+1;
    
    //    int filterLen = 0;
    
    // Temporary arrays
    //    float inputFloat[maxDataArraySize] = {0};
    //    float inputFloat3[3*maxDataArraySize] = {0};
    //    float outputFloat[maxDataArraySize] = {0};
    //    float correlationResult[2*maxDataArraySize+1] = {0};
    //
    //    float echo_filt[2*maxDataArraySize+1] = {0};
    //    float filterFloat[2*maxDataArraySize+1] = {0};
    //    float realFloat1[2*maxDataArraySize+1 / 2] = {0};
    //    float imagFloat1[2*maxDataArraySize+1 / 2] = {0};
    //    DSPSplitComplex splitCplxFloat1 = {realFloat1, imagFloat1};
    //
    //    float realFloat2[maxDataArraySize / 2] = {0};
    //    float imagFloat2[maxDataArraySize / 2] = {0};
    //    DSPSplitComplex splitCplxFloat2 = {realFloat2, imagFloat2};
    
    int filterLen = 11040; // 11041
    int16_t state[11040] = {0};
    int16_t tempData[11040] = {0};
    int16_t echo_filt[11040] = {0};
    //int filterLen = 0;
    
    
    
    
public:
    
    void setup();

    void filter(int16_t *outputData,
                const int16_t *inputData,
                int inputNumSamples);
    
};

#endif /* EchoCancel_hpp */


//    float realFilterFloat[maxDataArraySize / 2] = {0};
//    float imagFilterFloat[maxDataArraySize / 2] = {0};
//    DSPSplitComplex splitCplxFilterFloat = {realFilterFloat, imagFilterFloat};
