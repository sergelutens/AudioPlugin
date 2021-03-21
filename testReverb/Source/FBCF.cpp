//
//  FBCF.cpp
//
//  Created by Eric Tarr on 2/6/20.
//  Copyright © 2020 Eric Tarr. All rights reserved.
//

#include "FBCF.hpp"


FBCF::FBCF() {
    // 생성자

}
FBCF::FBCF(float delay, float speed) {
    // 
    fractionalDelay.setDelaySamples(delay);
    fractionalDelay.setSpeed(speed);

}
FBCF::FBCF(float delay, float speed, float apfDelay, float apfSpeed) {
    // 
    fractionalDelay.setDelaySamples(delay);
    fractionalDelay.setSpeed(speed);
    apf.setDelay(apfDelay);
    apf.setSpeed(apfSpeed);
    apf.setFeedbackGain(0.2f);// 올-패스 필터의 피드백 게인값을 직접 조절해줌
    apf.setDepth(3.f); // 모듈레이션 값을 작게 설정하자
}

// Destructor
FBCF::~FBCF() {
}


float FBCF::processSample(float x, int channel)
{
    float y;

    // 딜레이 라인 생성

	// FBCF에서는 무작위 피드백을 위해 음수로 주는 것도 좋다.
    // 
	float inDL1 = x + (-feedbackGain) * fb1[channel];


    //딜레이 라인의 아웃풋 생성
    float outDL1 = fractionalDelay.processSample(inDL1, channel);

  
    y = outDL1;

    // 피드백 값은 outDL1의 값이 됨.
	// 피드백 경로에 LPF를 걸어줌
    // fb1[channel] = lpf.processSample(outDL1, channel);
    fb1[channel] = apf.processSample(outDL1, channel);
    //

    return y;
}

void FBCF::setFs(float Fs) {
    this->Fs = Fs;
    fractionalDelay.setFs(Fs);
    apf.setFs(Fs);
}

void FBCF::setDepth(float depth) {

    this->depth = depth;
    fractionalDelay.setDepth(depth);

}

void FBCF::setFeedbackGain(float feedbackGain)
{
    this->feedbackGain = feedbackGain;
}