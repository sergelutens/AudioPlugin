//
//  FDN.cpp
//
//  Created by Eric Tarr on 2/6/20.
//  Copyright © 2020 Eric Tarr. All rights reserved.
//

#include "FDN.hpp"


FDN::FDN(){
    // 생성자

}


// Destructor
FDN::~FDN(){
}


float FDN::processSample(float x, int channel)
{
    float y;
    
    //딜레이 라인 생성
    float inDL1 = x + fb1[channel];
    float inDL2 = x + fb2[channel];
		float inDL3 = x + fb3[channel];
    float inDL4 = x + fb4[channel];

    //딜레이 라인의 아웃풋 생성
    float outDL1 = fractionalDelay1.processSample(inDL1, channel);
    float outDL2 = fractionalDelay2.processSample(inDL2, channel);
		float outDL3 = fractionalDelay1.processSample(inDL3, channel);
    float outDL4 = fractionalDelay2.processSample(inDL4, channel);

    // 아웃이 되는 딜레이 라인이 두 개이기 때문에 * 0.5
    y = 0.25 * (outDL1 + outDL2 + outDL3 + outDL4);
    
    // 피드백이 줄어들 지 않으므로 feedbackGain을 곱해 조금씩 줄어들도록 설정
    fb1[channel] = (-outDL2 + outDL3) * feedbackGain;
    fb2[channel] = (outDL1 + outDL4) * feedbackGain;
    fb3[channel] = (outDL1 + -outDL4) * feedbackGain;
    fb4[channel] = (-outDL2 + -outDL3) * feedbackGain;

    return y;
}

void FDN::setFs(float Fs){
    this->Fs = Fs;
    fractionalDelay1.setFs(Fs);
    fractionalDelay2.setFs(Fs);
}

void FDN::setDepth(float depth){

    this->depth = depth;
		fractionalDelay1.setDepth(depth);
    fractionalDelay2.setDepth(depth);
		fractionalDelay3.setDepth(depth);
    fractionalDelay4.setDepth(depth);
}

void FDN::setTime(float timeValue)
{
    feedbackGain = timeValue;
}