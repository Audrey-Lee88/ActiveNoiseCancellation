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

#include <Accelerate/Accelerate.h>

class EchoCancel {
    
    static const int maxDataArraySize = 8192;

    // Temporary arrays
    float inputFloat[maxDataArraySize] = {0};
    float outputFloat[maxDataArraySize] = {0};
    
public:
    
    void setup(void);

    void filter(int16_t *outputData, const int16_t *inputData,
              int inputNumSamples);
    
    
};

#endif /* EchoCancel_hpp */
