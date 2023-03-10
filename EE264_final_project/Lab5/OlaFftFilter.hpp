//
//  OlaFftFilter.hpp
//  EE264_lab
//
//  Created by EE Labs User on 2/16/23.
//

#ifndef OlaFftFilter_hpp
#define OlaFftFilter_hpp

#include <stdio.h>
#include <math.h>

#include <Accelerate/Accelerate.h>
#include "FftModule.hpp"

class OlaFftFilter {
    
    static const int maxFftSize = 2048;
    static const int maxDataArraySize = 8192;
    static const int olaFftSize = 1024;
    const int log2OlaFftSize = (int)log2((double)olaFftSize);
    
    static const int LowPassFilterLength = 101;
    const int16_t LowPassFilter[LowPassFilterLength] =
    {2, 2, -1, -5, -4, 5, 11, 3, -14, -17, 3, 28, 21, -19, -46, -15, 48, 62, -8, -87, -67, 54, 131, 46, -125, -165, 14, 217, 169, -123, -313, -117, 285, 386, -20, -492, -397, 269, 726, 291, -668, -959, 24, 1305, 1158, -798, -2557, -1292, 3688, 9615, 12262, 9615, 3688, -1292, -2557, -798, 1158, 1305, 24, -959, -668, 291, 726, 269, -397, -492, -20, 386, 285, -117, -313, -123, 169, 217, 14, -165, -125, 46, 131, 54, -67, -87, -8, 62, 48, -15, -46, -19, 21, 28, 3, -17, -14, 3, 11, 5, -4, -5, -1, 2, 2};
    
    FftModule fftModule = FftModule(maxFftSize);
    
    // Temporary arrays
    float inputFloat[maxDataArraySize] = {0};
    float outputFloat[maxDataArraySize] = {0};
    float realFloat1[maxDataArraySize / 2] = {0};
    float imagFloat1[maxDataArraySize / 2] = {0};
    DSPSplitComplex splitCplxFloat1 = {realFloat1, imagFloat1};
    float realFloat2[maxDataArraySize / 2] = {0};
    float imagFloat2[maxDataArraySize / 2] = {0};
    DSPSplitComplex splitCplxFloat2 = {realFloat2, imagFloat2};
    
    float filterFloat[maxDataArraySize] = {0};
    float realFilterFloat[maxDataArraySize / 2] = {0};
    float imagFilterFloat[maxDataArraySize / 2] = {0};
    DSPSplitComplex splitCplxFilterFloat = {realFilterFloat, imagFilterFloat};
    
    int16_t filterState[maxDataArraySize] = {0};
    int16_t tempData[maxDataArraySize] = {0};
    int16_t outputInt[maxDataArraySize] = {0};
    
public:
    
    void setup(void);

    void filter(int16_t *outputData, const int16_t *inputData,
              int inputNumSamples);
    
    
};


#endif /* OlaFftFilter_hpp */
