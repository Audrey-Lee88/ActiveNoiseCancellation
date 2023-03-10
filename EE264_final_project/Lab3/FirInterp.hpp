//
//  FirInterp.hpp
//
//  Created by Fernando Mujica on 1/15/18.
//  Copyright © 2018 Stanford University. All rights reserved.
//

#ifndef FirInterp_hpp
#define FirInterp_hpp

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>


const int UnitImpulseFirFilterLen = 1;   // Unit impulse
const int16_t UnitImpulseFirFilterCoeff[UnitImpulseFirFilterLen] = {32767};

/*
const int L3ZeroOrderHoldFirInterpFilterLen = 3;   // Zero-order hold
const int16_t L3FirInterpFilterCoeff[L3ZeroOrderHoldFirInterpFilterLen] = {32767, 32767, 32767};

const int L16ZeroOrderHoldFirInterpFilterLen = 16;  // Zero-order hold
const int16_t L16FirInterpFilterCoeff[L16ZeroOrderHoldFirInterpFilterLen] = {32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767};
 */

// We chose to use the windowing method because the frequency resp. the ripples decayed as freq. increased in the stop band which means it canceled out those freq. better

const int L3FirInterpFilterLen = 101;
// Equiripple

const int16_t L3FirInterpFilterCoeff[L3FirInterpFilterLen] = {97, 212, -60, -56, -98, 4, 102, 113, 0, -132, -142, 0, 164, 176, 0, -203, -217, 0, 249, 266, 0, -304, -324, 0, 370, 395, 0, -452, -484, 0, 556, 596, 0, -691, -747, 0, 879, 959, 0, -1161, -1291, 0, 1642, 1890, 0, -2677, -3361, 0, 6762, 13543, 16384, 13543, 6762, 0, -3361, -2677, 0, 1890, 1642, 0, -1291, -1161, 0, 959, 879, 0, -747, -691, 0, 596, 556, 0, -484, -452, 0, 395, 370, 0, -324, -304, 0, 266, 249, 0, -217, -203, 0, 176, 164, 0, -142, -132, 0, 113, 102, 4, -98, -56, -60, 212, 97};

// Window

//const int16_t L3FirInterpFilterCoeff[L3FirInterpFilterLen] = {24, 28, 0, -39, -45, 0, 58, 65, 0, -82, -91, 0, 112, 123, 0, -149, -163, 0, 194, 211, 0, -249, -270, 0, 317, 343, 0, -402, -434, 0, 509, 551, 0, -649, -707, 0, 843, 926, 0, -1132, -1263, 0, 1619, 1870, 0, -2662, -3349, 0, 6753, 13535, 16378, 13535, 6753, 0, -3349, -2662, 0, 1870, 1619, 0, -1263, -1132, 0, 926, 843, 0, -707, -649, 0, 551, 509, 0, -434, -402, 0, 343, 317, 0, -270, -249, 0, 211, 194, 0, -163, -149, 0, 123, 112, 0, -91, -82, 0, 65, 58, 0, -45, -39, 0, 28, 24};


const int L16FirInterpFilterLen = 401;
// Equiripple

const int16_t L16FirInterpFilterCoeff[L16FirInterpFilterLen] = {125, 18, 18, 17, 15, 12, 7, 2, -4, -10, -17, -24, -31, -37, -42, -46, -49, -50, -49, -46, -41, -34, -25, -15, -3, 9, 22, 35, 47, 58, 67, 74, 78, 79, 77, 72, 63, 51, 36, 18, -1, -22, -43, -64, -84, -101, -116, -126, -133, -134, -130, -121, -106, -86, -61, -33, -2, 31, 64, 97, 127, 154, 176, 192, 201, 203, 196, 181, 158, 127, 90, 47, 0, -49, -99, -148, -193, -232, -264, -288, -300, -302, -291, -268, -234, -189, -133, -70, -2, 70, 143, 213, 278, 335, 381, 414, 431, 433, 417, 384, 334, 269, 189, 99, 1, -102, -205, -305, -397, -477, -542, -588, -613, -615, -592, -545, -474, -381, -269, -141, -2, 144, 290, 431, 561, 675, 767, 833, 869, 871, 839, 773, 672, 541, 381, 200, 1, -206, -415, -618, -805, -970, -1104, -1201, -1254, -1261, -1217, -1123, -979, -790, -558, -293, -2, 305, 617, 921, 1205, 1457, 1666, 1821, 1911, 1931, 1876, 1741, 1528, 1240, 883, 466, 2, -497, -1012, -1526, -2018, -2469, -2857, -3164, -3370, -3458, -3414, -3227, -2888, -2394, -1745, -944, -2, 1070, 2255, 3532, 4877, 6265, 7666, 9053, 10394, 11660, 12823, 13857, 14738, 15446, 15963, 16279, 16385, 16279, 15963, 15446, 14738, 13857, 12823, 11660, 10394, 9053, 7666, 6265, 4877, 3532, 2255, 1070, -2, -944, -1745, -2394, -2888, -3227, -3414, -3458, -3370, -3164, -2857, -2469, -2018, -1526, -1012, -497, 2, 466, 883, 1240, 1528, 1741, 1876, 1931, 1911, 1821, 1666, 1457, 1205, 921, 617, 305, -2, -293, -558, -790, -979, -1123, -1217, -1261, -1254, -1201, -1104, -970, -805, -618, -415, -206, 1, 200, 381, 541, 672, 773, 839, 871, 869, 833, 767, 675, 561, 431, 290, 144, -2, -141, -269, -381, -474, -545, -592, -615, -613, -588, -542, -477, -397, -305, -205, -102, 1, 99, 189, 269, 334, 384, 417, 433, 431, 414, 381, 335, 278, 213, 143, 70, -2, -70, -133, -189, -234, -268, -291, -302, -300, -288, -264, -232, -193, -148, -99, -49, 0, 47, 90, 127, 158, 181, 196, 203, 201, 192, 176, 154, 127, 97, 64, 31, -2, -33, -61, -86, -106, -121, -130, -134, -133, -126, -116, -101, -84, -64, -43, -22, -1, 18, 36, 51, 63, 72, 77, 79, 78, 74, 67, 58, 47, 35, 22, 9, -3, -15, -25, -34, -41, -46, -49, -50, -49, -46, -42, -37, -31, -24, -17, -10, -4, 2, 7, 12, 15, 17, 18, 18, 125};



// Window
//const int16_t L16FirInterpFilterCoeff[L16FirInterpFilterLen] = {6, 7, 7, 7, 6, 5, 4, 2, 0, -2, -5, -8, -11, -14, -16, -18, -20, -21, -21, -20, -18, -15, -11, -6, 0, 6, 13, 20, 26, 32, 38, 42, 44, 46, 45, 42, 37, 31, 22, 12, 0, -13, -26, -39, -52, -63, -73, -80, -85, -86, -84, -78, -69, -56, -40, -21, 0, 23, 46, 69, 91, 110, 127, 139, 146, 148, 144, 133, 117, 95, 67, 35, 0, -38, -76, -113, -149, -180, -206, -225, -236, -238, -230, -213, -186, -151, -107, -56, 0, 59, 119, 177, 231, 279, 319, 347, 364, 366, 354, 327, 285, 230, 162, 85, 0, -89, -179, -267, -349, -420, -479, -521, -544, -547, -528, -487, -424, -342, -241, -126, 0, 132, 266, 396, 516, 621, 707, 770, 804, 808, 780, 719, 627, 505, 356, 186, 0, -196, -394, -586, -765, -922, -1051, -1145, -1198, -1205, -1165, -1077, -940, -759, -537, -281, 0, 297, 600, 895, 1173, 1420, 1625, 1777, 1868, 1889, 1836, 1706, 1499, 1217, 867, 458, 0, -492, -1001, -1509, -1997, -2444, -2831, -3136, -3342, -3432, -3390, -3206, -2870, -2380, -1735, -939, 0, 1069, 2251, 3525, 4869, 6256, 7657, 9043, 10385, 11653, 12818, 13853, 14735, 15444, 15962, 16279, 16385, 16279, 15962, 15444, 14735, 13853, 12818, 11653, 10385, 9043, 7657, 6256, 4869, 3525, 2251, 1069, 0, -939, -1735, -2380, -2870, -3206, -3390, -3432, -3342, -3136, -2831, -2444, -1997, -1509, -1001, -492, 0, 458, 867, 1217, 1499, 1706, 1836, 1889, 1868, 1777, 1625, 1420, 1173, 895, 600, 297, 0, -281, -537, -759, -940, -1077, -1165, -1205, -1198, -1145, -1051, -922, -765, -586, -394, -196, 0, 186, 356, 505, 627, 719, 780, 808, 804, 770, 707, 621, 516, 396, 266, 132, 0, -126, -241, -342, -424, -487, -528, -547, -544, -521, -479, -420, -349, -267, -179, -89, 0, 85, 162, 230, 285, 327, 354, 366, 364, 347, 319, 279, 231, 177, 119, 59, 0, -56, -107, -151, -186, -213, -230, -238, -236, -225, -206, -180, -149, -113, -76, -38, 0, 35, 67, 95, 117, 133, 144, 148, 146, 139, 127, 110, 91, 69, 46, 23, 0, -21, -40, -56, -69, -78, -84, -86, -85, -80, -73, -63, -52, -39, -26, -13, 0, 12, 22, 31, 37, 42, 45, 46, 44, 42, 38, 32, 26, 20, 13, 6, 0, -6, -11, -15, -18, -20, -21, -21, -20, -18, -16, -14, -11, -8, -5, -2, 0, 2, 4, 5, 6, 7, 7, 7, 6};


class FirInterp {
    
    static const int maxFilterLength = 512;
    static const int maxDataArraySize = 8192;
    
    int filterLen = 0;
    
    int16_t coeff[maxFilterLength] = {0};
    int16_t state[maxDataArraySize] = {0};
    int16_t tempData[maxDataArraySize] = {0};
    

    
    
public:
    
    void setup(const int16_t *filterCoeff, int inFilterLen);
    
    void filter(int16_t *outputData,
                const int16_t *inputData,
                int inputNumSamples,
                int upSampleFactor,
                int downSampleFactor = 1);
    
};

#endif /* FirInterp_hpp */


