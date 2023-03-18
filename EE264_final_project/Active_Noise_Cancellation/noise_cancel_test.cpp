//
//  noise_cancel_test.cpp
//  EE264_lab
//
//  Created by EE Labs User on 3/17/23.
//

//#include <stdio.h>
//
//// Using Lab 3; FIR method
//void NoiseCancel::filter(int16_t *outputData,
//                         const int16_t *inputData,
//                         int inputNumSamples,
//                         int upSampleFactor,
//                         int downSampleFactor){
////    int var_int = (int)var;
////    int mu_int = (int)mu;
//
//    // Un-interleave the input data
//    // temp Data is signal plus noise
//    for (int i = 0; i < inputNumSamples/2; i++) {
//        tempData[i] = inputData[upSampleFactor*i];
//        for(int k = 1; k < upSampleFactor; k++){
//            tempData[i+k] = inputData[upSampleFactor*i];
//        }
//    }
//    //temp Data_Int is noise ref
//    for (int i = 0; i < inputNumSamples/2; i++) {
//        temp_Int_Noise[i] = inputData[upSampleFactor*i+1];
//        for(int k = 1; k < upSampleFactor; k++){
//            temp_Int_Noise[i+k] = inputData[upSampleFactor*i+1];
//        }
//    }
//
//    //OLD UN-INTERLEAVE
//    //    int16_t signal_plus_noise[inputNumSamples];
//    //    int16_t noise_reference[inputNumSamples];
////    for (int i = 0; i < inputNumSamples/2; i++) {
////        signal_plus_noise[i] = inputData[2*i];
////    }
////    for (int i = 0; i < inputNumSamples/2; i++) {
////        noise_reference[i] = inputData[2*i+1];
////    }
////        int16_t y[inputNumSamples/2];
////        int16_t e[inputNumSamples/2];
//
//
//        int16_t temp_r[filterLen];
//
//        int32_t mult_mu_e = 0;
//        int16_t mult_mu_e_shift;
//
//
//
////      int16_t tempData1[maxDataArraySize];
//
//      int N = inputNumSamples*upSampleFactor;
//
//    // FIR FILTER lab 3; Upsample
//      //if (inputNumSamples*upSampleFactor <= maxDataArraySize){
////      for(int n = 0; n < inputNumSamples; n++) {
////          tempData[n*upSampleFactor] = inputData[n];
////          for(int k = 1; k < upSampleFactor; k++){
////              tempData[n*upSampleFactor+k] = 0;
////          }
////      }
//
//      // for 0 < n < K-1
//      for (int n = 0; n < filterLen - 1; n++){
//          int32_t acc = 0;
//          // added temp_r (noise ref that is grabbed backwards) to here and below for loops
//          for (int k = 0; k < n + 1; k++){
//              //temp_r[k] = temp_Int_Noise[n - k];
//              acc += temp_Int_Noise[n - k] * coeff[k];
//          }
//          for (int k = n + 1; k < filterLen; k++){
//              //temp_r[k] = state[k-(n+1)];
//              acc += state[k-(n+1)] * coeff[k];
//          }
//          // prev FIR
////          tempData1[n] = acc >> 15;
//
//
//          // LMS coefficient adaptation
//          int y = acc >> 15;
//
//          for (int i = 0; i < filterLen; i++){
//              if (n - i >= 0){
//                  temp_r[i] = temp_Int_Noise[n-i];
//              }
//              else{
//                  temp_r[i] = state[filterLen - 1 - i];
//              }
//          }
//
//          // w = coeff; y = acc (tempData1 is bitshifted y); tempData = sig_plus_noise
//          // OR, do we want to leave y as is and then bitshift e?????
//          //find e
//          e[n] = tempData[n] - y;
//
//          // update w
//          mult_mu_e = mu*e[n];
//          mult_mu_e_shift = mult_mu_e >> 15;
//          int32_t acc1 = 0;
////          mult_mu_e_r = (int)mult_mu_e_r;
//          for (int i = 0; i < filterLen; i++) {
//              acc1 = (coeff[i] << 15) + (mult_mu_e_shift*temp_r[i]);
//              coeff[i] = acc1 >> 15;
////              std::cout << coeff[i] << " | ";
//          }
//
//
//      }
//      for (int n = filterLen - 1; n < inputNumSamples/2; n++){
//          int32_t acc = 0;
//          for (int k =0; k < filterLen; k++){
//              //temp_r[k] = temp_Int_Noise[n - k];
//              acc += temp_Int_Noise[n - k] * coeff[k];
//          }
////          tempData1[n] = acc >> 15;
//          // LMS coefficient adaptation
//          int y = acc >> 15;
//          e[n] = tempData[n] - y;
//
//          for (int i = 0; i < filterLen; i++){
//            temp_r[i] = temp_Int_Noise[n-i];
//          }
//          // update w
//          mult_mu_e = mu*e[n];
//          mult_mu_e_shift = mult_mu_e >> 15;
//          int32_t acc1 = 0;
//          for (int i = 0; i < filterLen; i++) {
//              acc1 = (coeff[i] << 15) + (mult_mu_e_shift*temp_r[i]);
//              coeff[i] = acc1 >> 15;
//          }
//      }
////      int e_bitshift = 0;
//      int n = 0;
//
////    std::ofstream ofile;
////          ofile.open("/Users/eelabsuser/Documents/Audrey/ActiveNoiseCancellation/test_data.txt", std::ios::app);
//
////          for (int i = 0; i < maxDataArraySize; i++) {
////              ofile << e[i] << std::endl;
////          }
////          ofile.close();
//
////      while(downSampleFactor * n < inputNumSamples * upSampleFactor){
//////          outputData[n] = tempData1[downSampleFactor*n]; //what it once was in FIR
//////          e_bitshift  = e[downSampleFactor*n];
////          outputData[n] = e[downSampleFactor*n];
//////          ofile << outputData[n] << std::endl;
////          n++;
////      }
//    for (int i = 0; i < inputNumSamples; i++){
//        if (i % 2 == 0){
//            outputData[2*i] = e[i];
//            outputData[2*i + 1] = e[i];
//        }
//    }
////    ofile.close();
//    //    for (int i = 0; i < inputNumSamples; i++){
//    //        outputData[i] = e[i];
//    //    }
//
//
//      for (int i = 0; i < filterLen - 1; i++){
//          state[i] = temp_Int_Noise[inputNumSamples - 1 - i];
//      }
//  }


// Using Lab 3; FIR method
//void NoiseCancel::filter(int16_t *outputData,
//                         const int16_t *inputData,
//                         int inputNumSamples,
//                         int upSampleFactor,
//                         int downSampleFactor){
////    int var_int = (int)var;
////    int mu_int = (int)mu;
//
//    // Un-interleave the input data
//    // temp Data is signal plus noise
//    for (int i = 0; i < inputNumSamples/2; i++) {
//        tempData[i] = inputData[upSampleFactor*i];
//        for(int k = 1; k < upSampleFactor; k++){
//            tempData[i+k] = inputData[upSampleFactor*i];
//        }
//    }
//    //temp Data_Int is noise ref
//    for (int i = 0; i < inputNumSamples/2; i++) {
//        temp_Int_Noise[i] = inputData[upSampleFactor*i+1];
//        for(int k = 1; k < upSampleFactor; k++){
//            temp_Int_Noise[i+k] = inputData[upSampleFactor*i+1];
//        }
//    }
//
//    //OLD UN-INTERLEAVE
//    //    int16_t signal_plus_noise[inputNumSamples];
//    //    int16_t noise_reference[inputNumSamples];
////    for (int i = 0; i < inputNumSamples/2; i++) {
////        signal_plus_noise[i] = inputData[2*i];
////    }
////    for (int i = 0; i < inputNumSamples/2; i++) {
////        noise_reference[i] = inputData[2*i+1];
////    }
////        int16_t y[inputNumSamples/2];
////        int16_t e[inputNumSamples/2];
//
//
//        int16_t temp_r[filterLen];
//
//        int32_t mult_mu_e = 0;
//        int16_t mult_mu_e_shift;
//
//
//
////      int16_t tempData1[maxDataArraySize];
//
//      int N = inputNumSamples*upSampleFactor;
//
//    // FIR FILTER lab 3; Upsample
//      //if (inputNumSamples*upSampleFactor <= maxDataArraySize){
////      for(int n = 0; n < inputNumSamples; n++) {
////          tempData[n*upSampleFactor] = inputData[n];
////          for(int k = 1; k < upSampleFactor; k++){
////              tempData[n*upSampleFactor+k] = 0;
////          }
////      }
//
//      // for 0 < n < K-1
//      for (int n = 0; n < filterLen - 1; n++){
//          int32_t acc = 0;
//          // added temp_r (noise ref that is grabbed backwards) to here and below for loops
//          for (int k = 0; k < n + 1; k++){
//              //temp_r[k] = temp_Int_Noise[n - k];
//              acc += temp_Int_Noise[n - k] * coeff[k];
//          }
//          for (int k = n + 1; k < filterLen; k++){
//              //temp_r[k] = state[k-(n+1)];
//              acc += state[k-(n+1)] * coeff[k];
//          }
//          // prev FIR
////          tempData1[n] = acc >> 15;
//
//
//          // LMS coefficient adaptation
//          int y = acc >> 15;
//
//          for (int i = 0; i < filterLen; i++){
//              if (n - i >= 0){
//                  temp_r[i] = temp_Int_Noise[n-i];
//              }
//              else{
//                  temp_r[i] = state[filterLen - 1 - i];
//              }
//          }
//
//          // w = coeff; y = acc (tempData1 is bitshifted y); tempData = sig_plus_noise
//          // OR, do we want to leave y as is and then bitshift e?????
//          //find e
//          e[n] = tempData[n] - y;
//
//          // update w
//          mult_mu_e = mu*e[n];
//          mult_mu_e_shift = mult_mu_e >> 15;
//          int32_t acc1 = 0;
////          mult_mu_e_r = (int)mult_mu_e_r;
//          for (int i = 0; i < filterLen; i++) {
//              acc1 = (coeff[i] << 15) + (mult_mu_e_shift*temp_r[i]);
//              coeff[i] = acc1 >> 15;
////              std::cout << coeff[i] << " | ";
//          }
//
//
//      }
//      for (int n = filterLen - 1; n < inputNumSamples/2; n++){
//          int32_t acc = 0;
//          for (int k =0; k < filterLen; k++){
//              //temp_r[k] = temp_Int_Noise[n - k];
//              acc += temp_Int_Noise[n - k] * coeff[k];
//          }
////          tempData1[n] = acc >> 15;
//          // LMS coefficient adaptation
//          int y = acc >> 15;
//          e[n] = tempData[n] - y;
//
//          for (int i = 0; i < filterLen; i++){
//            temp_r[i] = temp_Int_Noise[n-i];
//          }
//          // update w
//          mult_mu_e = mu*e[n];
//          mult_mu_e_shift = mult_mu_e >> 15;
//          int32_t acc1 = 0;
//          for (int i = 0; i < filterLen; i++) {
//              acc1 = (coeff[i] << 15) + (mult_mu_e_shift*temp_r[i]);
//              coeff[i] = acc1 >> 15;
//          }
//      }
////      int e_bitshift = 0;
//      int n = 0;
//
////    std::ofstream ofile;
////          ofile.open("/Users/eelabsuser/Documents/Audrey/ActiveNoiseCancellation/test_data.txt", std::ios::app);
//
////          for (int i = 0; i < maxDataArraySize; i++) {
////              ofile << e[i] << std::endl;
////          }
////          ofile.close();
//
////      while(downSampleFactor * n < inputNumSamples * upSampleFactor){
//////          outputData[n] = tempData1[downSampleFactor*n]; //what it once was in FIR
//////          e_bitshift  = e[downSampleFactor*n];
////          outputData[n] = e[downSampleFactor*n];
//////          ofile << outputData[n] << std::endl;
////          n++;
////      }
//    for (int i = 0; i < inputNumSamples; i++){
//        if (i % 2 == 0){
//            outputData[2*i] = e[i];
//            outputData[2*i + 1] = e[i];
//        }
//    }
////    ofile.close();
//    //    for (int i = 0; i < inputNumSamples; i++){
//    //        outputData[i] = e[i];
//    //    }
//
//
//      for (int i = 0; i < filterLen - 1; i++){
//          state[i] = temp_Int_Noise[inputNumSamples/2 - 1 - i];
//      }
//  }
