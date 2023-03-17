//
//  NoiseCancel.hpp
//  EE264_lab
//
//  Created by EE Labs User on 3/9/23.
//

#ifndef NoiseCancel_hpp
#define NoiseCancel_hpp

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <Accelerate/Accelerate.h>
#include "matrix.h"
//#include </opt/homebrew/l,/ib/armadillo.h>


//const int L32FirInterpFilterLen = 32;
//const int16_t L32FirInterpFilterCoeff[L32FirInterpFilterLen] = {-38, -46, 64, 96, -143, -209, 296, 409, -555, -745, 998, 1349, -1877, -2786, 4823, 14747, 14747, 4823, -2786, -1877, 1349, 998, -745, -555, 409, 296, -209, -143, 96, 64, -46, -38};

class NoiseCancel {
    
    static const int maxFilterLength = 512;
    static const int maxDataArraySize = 8192;
    
    int filterLen = 0;
    int samp_len = 256;
    
    int16_t coeff[maxFilterLength] = {0};
    int16_t state[maxDataArraySize] = {0};
    int16_t tempData[maxDataArraySize] = {0};
    float inputFloat[maxDataArraySize] = {0};
    float tempData_Float[maxDataArraySize] = {0};
    float tempInput_Float[maxDataArraySize] = {0};
    int16_t tempData_Int[maxDataArraySize] = {0};
    
    float var = 2e-5;
    int L = 25;
    
    float mu = 0.001/((L+1)*var);

    
public:
    
    void setup(const int16_t *filterCoeff, int samp_len);
    
    void filter(int16_t *outputData,
                const int16_t *inputData,
                int inputNumSamples);
    
};


#endif /* NoiseCancel_hpp */
