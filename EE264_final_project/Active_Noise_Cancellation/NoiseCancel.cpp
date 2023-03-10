//
//  NoiseCancel.cpp
//  EE264_lab
//
//  Created by EE Labs User on 3/9/23.
//

#include "NoiseCancel.hpp"

using namespace std;

void NoiseCancel::setup(const int16_t *filterCoeff,
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

void NoiseCancel::filter(int16_t *outputData,
                       const int16_t *inputData,
                       const int16_t *desiredSignal,
                       int inputNumSamples){
    int16_t tempData1[maxDataArraySize];
    
    float correlationResult = (inputNumSamples);

    vDSP_conv(inputData,
              1,
              inputData,
              1, // The stride through the filter vector.
              &correlationResult,
              1,
              inputNumSamples, maxDataArraySize);
    
    /*
     D = s; //desired signal
     A = signal_plus_noise; // inputdata

     n = numel(D); // desired signal len (same as len of input data)

     M = 25;
     w = zeros(M,1);
     wi = zeros(M,1);
     R = [];
     k = 1;
     r = xcorr(A)
     rr = [];

     for i = 1:1:M
         rr(i) = r(n-1+1);
     end

     R = toeplitz(rr);
     ei = max(eig(R));
     u = 1/ei;
     p = xcorr(D, A);

     for i = 1:1:M
         P(i) = p(n-i+1);
     end

     for i = 1:10
         wi = w + u*(P'-R*w);
         w = wi;
     end

     % find the estimate
     Est = zeros(n, 1)
     for i = M:n
         j = A(i:-1:i-M+1);
         Est(i) = (wi)'*(j)';
     end

     % error signal
     Err = Est' - D;
     
     */
    
    
    
}
