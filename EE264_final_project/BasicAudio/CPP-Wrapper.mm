//
//  CPP-Wrapper.m
//
//  Created by Prateek Murgai on 12/7/17.
//  Copyright © 2017 Stanford University. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CPP-Wrapper.h"
#include "AudioProcessing.hpp"

AudioProcessing audioProcessing;


@implementation AudioProcessingWrapper

- (void)processAudio: (int16_t*) outputData numOutputFrames:(int) numOutputFrames inputData:(int16_t*) inputData micOn:(bool) micOn fileData:(int16_t*) fileData numFileFrames:(int) numFileFrames test:(bool) test mode:(int) mode {
    
    audioProcessing.processAudio(outputData, numOutputFrames, inputData, micOn, fileData, numFileFrames, test, mode);
}
@end


@implementation AudioProcessingSetupWrapper

- (void)setup: (int) outputSamplingRate numInputFrames:(int) numInputFrames upSampleFactor:(int) upSampleFactor downSampleFactor:(int) downSampleFactor inputFileName:(const char*) inputFileName {
    
    audioProcessing.setup(outputSamplingRate, numInputFrames, upSampleFactor, downSampleFactor, inputFileName);
}
@end


@implementation AudioProcessingSetupCodecWrapper

- (void)setupCodec: (float) param1 param2:(int) param2 param3:(int) param3 {
    
    audioProcessing.setupCodec(param1, param2, param3);
}
@end
 

@implementation AudioProcessingCleanupWrapper

- (void)cleanup {
    
    audioProcessing.cleanup();
}
@end


@implementation AudioProcessingNumFileSamplesNeededForWrapper

- (int)numFileSamplesNeededFor: (int) numInputFrames {
    
    return(audioProcessing.fileNumSamplesNeededFor(numInputFrames));
}
@end


