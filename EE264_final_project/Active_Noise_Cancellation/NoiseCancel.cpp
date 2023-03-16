////
////  NoiseCancel.cpp
////  EE264_lab
////
////  Created by EE Labs User on 3/9/23.
////
//
//#include "NoiseCancel.hpp"
//
//using namespace std;
//
//void NoiseCancel::setup(const int16_t *filterCoeff,
//                      int inFilterLen){
//    if (inFilterLen < maxFilterLength){
//        filterLen = inFilterLen;
//    }
//    else{
//        filterLen = maxFilterLength;
//    }
//    for (int i = 0; i < filterLen; i++){
//        coeff[i] = filterCoeff[i]; // copy the filterCoeff array val to coeff array
//    }
//    for (int i = 0; i < maxFilterLength; i++){
//        state[i] = 0; // clear filter state array
//    }
//}
//
//void NoiseCancel::filter(int16_t *outputData,
//                       const int16_t *inputData,
//                       int inputNumSamples){
//    // uninterleave the data
//    const int length = inputNumSamples/2;
//    float signal_plus_noise[length];
//    float noise_reference[length];
////    int n = inputNumSamples/2;
//
//    for (int ax = 0; ax < length; ax += 2){
//        signal_plus_noise[ax] = inputData[ax];
//        noise_reference[ax+1] = inputData[ax+1];
//    }
//
//
//    // setup for the gradient descent algorithm
//    int M = 25;
//
////    Matrix R(25, 25, 0);
////    float rr[M];
////
////    float r[length-1];
////    vDSP_conv(r, 1, signal_plus_noise, 1, signal_plus_noise, 1, n, n);
////
////    for (int i = 0; i < M; i++) {
////        rr[i] = r[n - i + 1];
////    }
////
////
////    // turn the vector into toeplitz
////    // right half
////    for (int i = 0; i < M; i++) {
////        for (int j = 0; j < M; j++) {
////            R(i, j) = rr[j-i];
////        }
////        for (int j = M-1; j >= 0; j--) {
////            R(j, i) = rr[j-i];
////        }
////    }
////
////    int errorCode;
//////    double eigenvalue = 0.0;
////    int row = R.getRows();
////    double tolerance = 0.5;
////
////    // Power Iteration to find an eigenvalue
//////    tie(R, eigenvalue, errorCode) = R.powerIter(row, tolerance);
//////    std::cout << eigenvalue;
////
//////    float u = 1/eigenvalue;
////    double eigenvalue = 1;
////    Matrix u(1,1,0);
////    u(0,0) = 1/eigenvalue;
////
//////    float P[M];
////    Matrix P(25, 1, 0);
////    for (int i = 0; i < M; i++) {
////        P(i,0) = r[n - i + 1];
////    }
////
//////    float w[M];
//////    float wi[M];
////    Matrix w(25, 1, 0);
////    Matrix wi(25, 1, 0);
////    Matrix inter(25,1,0);
////
////    Matrix inter2(25,1,0);
////
////    // initialize w to be empty zeros always at start of loops
////    for (int m = 0; m < M; m++) {
////        w(m, 0) = 0;
////    }
////
////    for (int i = 0; i<25; i++) {
////        std::cout << P(i,0);
////    }
////
////
////    // update wi based on w, then update w, and finally get wi after loops
////    for (int i = 0; i < 10; i++) {
//////        inter = R * w;
//////        inter2 = P - inter;
//////        wi = w + inter2;
//////        for (int idx = 0; idx < M; idx++) {
////            inter = R * w;
////
////            for (int m = 0; m < M; m++) {
////                wi(m,0) = w(m,0) + (P(m,0) - inter(m,0))* u(0,0);
////            }
////            for (int m = 0; m < M; m++) {
////                w(m, 0) = wi(m, 0);
////            }
//////        }
////    }
////
////    // keep wi and move on
////    //
////    Matrix est(length,1,0);
////    Matrix inter_a(1,25,0);
////
////
////    // initialize est to be empty zeros always at start of loops
////    for (int m = 0; m < length; m++) {
////        est(m, 0) = 0;
////    }
////
////    int access_index = 0;
////    for (int i = 0; i < length; i++) {
//////        j = A(i:-1:i-M+1);
////        // i - M to i backwards
////        for (int a = length - M - i; a < -i; a--) {
////            if (a < 0) {
////                access_index = length - (a-1);
////            }
////            else {
////                access_index = a;
////            }
////            inter_a(0, access_index) = signal_plus_noise[access_index];
//////            est(i,0) += inter_a(0,a) * wi(a,0);
////        }
////
////        for (int j=0; j < M; j++) {
////            est(i,0) += inter_a(0,j) * wi(j,0);
////        }
////    }
//
////    // convert matrix est to outputdata (arr)
////    for (int i = 0; i < length; i++) {
////        outputData[i] = est(i,0);
////    }
//
//#define mu 0.2f                //convergence rate
//#define M 25                    //order of filter
//#define I 256                //number of samples
//
//
//
//    // New try
//    long T, n = 0;
//    double D, Y, E;
//    double W[M] = { 0.0 };
//    double X[M] = { 0.0 };
//
//    double Desired[I] = { 0.0 };
//
//    for (int i = 0; i < I; i++)
//        for (int j = 0; j < M; j++)
//        if (i - j >= 0)
//            Desired[i] += signal_plus_noise[i - j] - noise_reference[i - j];
//
//    for (long T = 0; T < length; T++)
//        {
//                for (int m = T; m > T - M; m--){
//                    if (m >= 0)
//                        X[M + (m - T) - 1] = signal_plus_noise[m];    //X new input sample for
//                                        //LMS filter
//                    else break;
//                }
//
//                D = Desired[T];                    //desired signal
//                Y = 0;                        //filter’output set to zero
//
//                for (int i = 0; i < M; i++)
//                    Y += (W[i] * X[i]);            //calculate filter output
//
//                E = D - Y;                    //calculate error signal
//
//                for (int i = 0; i < M; i++)
//                    W[i] = W[i] + (mu * E * X[i]);        //update filter coefficients
//
//                std::cout << "Y_out" << (float)T << Y << "\n";
//                std::cout << "error" << (float)T << E << "\n";
//
//            // maybe?
//            outputData[T] = E;
//        }
//
//        for (int i = 0; i < M; i++)
//            std::cout << "weights" << i << W[i] << "\n";
//
//
//
//}
