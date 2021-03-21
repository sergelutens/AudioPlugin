//
//  APF.cpp
//
//  Created by Eric Tarr on 2/6/20.
//  Copyright © 2020 Eric Tarr. All rights reserved.
//

#include "APF.hpp"


APF::APF() {
    // 생성자

}


APF::APF(float delay, float speed) {
    // 
    fractionalDelay1.setDelaySamples(delay);
    fractionalDelay2.setDelaySamples(delay);
    fractionalDelay1.setSpeed(speed);
    fractionalDelay2.setSpeed(speed);

}

// Destructor
APF::~APF() {
}


float APF::processSample(float x, int channel)
{
    

    // 딜레이 라인 생성
    // 피드백 게인 미리 적용
	/*
    float inDL1 = x + feedbackGain2 * fb2[channel];
    float outLPF = lpf.processSample(inDL1, channel);

    float inDL11 = (outLPF * (1 - feedbackGain2)) + (feedbackGain2 * fb1[channel]);
    float outDL1 = fractionalDelay1.processSample(inDL11, channel);
    fb1[channel] = outDL1;
	
    float inDL2 = outDL1 + (inDL11 * (-feedbackGain1));
    float outDL2 = fractionalDelay2.processSample(inDL2, channel);
    fb2[channel] = outDL2;
	
    y = outDL2 + (inDL1 * (-feedbackGain1));*/
    float y;

    // 딜레이 라인 생성
    // 피드백 게인 미리 적용
    float inDL1 = x + (-feedbackGain1) * fb1[channel];
	inDL1 = lpf.processSample(inDL1, channel);

    //딜레이 라인의 아웃풋 생성
    float outDL1 = fractionalDelay1.processSample(inDL1, channel);

    //  원래 인풋의 신호 + 피드백 게인 블록을 통과한 신호
    y = outDL1 + feedbackGain1 * inDL1;

    // 피드백 값은 outDL1의 값이 됨.
    fb1[channel] = outDL1;

    return y;
}

void APF::setFs(float Fs) {
    this->Fs = Fs;
    fractionalDelay1.setFs(Fs);
    fractionalDelay2.setFs(Fs);
}

void APF::setDepth(float depth) {

    this->depth = depth;
    fractionalDelay1.setDepth(depth);
    fractionalDelay2.setDepth(depth);

}

void APF::setFeedbackGain(float feedbackGain)
{
    this->feedbackGain1 = feedbackGain;
    this->feedbackGain1 = feedbackGain;
}