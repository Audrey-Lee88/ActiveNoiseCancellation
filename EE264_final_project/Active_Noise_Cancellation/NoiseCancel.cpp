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
                       int inputNumSamples){
    // uninterleave the data
    float signal_plus_noise[inputNumSamples/2];
    float noise_reference[inputNumSamples/2];
    int n = inputNumSamples/2;
    
    for (int ax = 0; ax < inputNumSamples-2; ax += 2){
        signal_plus_noise[ax] = inputData[ax];
        noise_reference[ax+1] = inputData[ax+1];
    }
    
    // setup for the gradient descent algorithm
    int M = 25;
    float w[M];
    float wi[M];
    Matrix R(25, 25, 0);
    int k = 1;
    float rr[M];
    
    float r[inputNumSamples-1];
    vDSP_conv(r, 1, signal_plus_noise, 1, signal_plus_noise, 1, n, n);
    
    for (int i = 0; i < M; i++) {
        rr[i] = r[n - i + 1];
    }
    
    // turn the vector into toeplitz
    // right half
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            R(i, j) = rr[j-i];
        }
        for (int j = M-1; j >= 0; j--) {
            R(j, i) = rr[j-i];
        }
    }
    
    int errorCode;
    double eigenvalue = 0.0;
    int row = R.getRows();
    double tolerance = 0.5;
    
    // Power Iteration to find an eigenvalue
    tie(R, eigenvalue, errorCode) = R.powerIter(row, tolerance);
    
    float u = 1/eigenvalue;
    
    float P[M];
    for (int i = 0; i < M; i++) {
        P[i] = r[n - i + 1];
    }
    
//    for (i = 0; i < 10; i++) {
//        for (idx = 0; idx < M; idx++) {
//            wi[idx] =
//        }
//    }
    
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
