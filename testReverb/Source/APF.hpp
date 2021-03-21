//
//  APF.hpp
//
//  Created by Eric Tarr on 2/6/20.
//  Copyright © 2020 Eric Tarr. All rights reserved.
//

#ifndef APF_hpp
#define APF_hpp

#include "FractionalDelay.hpp" // APF 내부에서 Fractional Delay 블록을 사용해야하기 때문에
#include "LPF.hpp"
using namespace std;

class APF {

public:

    // Constructor function (special function - no return type, name = Class name)
    APF();

    // APF의 딜레이 값과 스피드를 만들어 줌
    APF(float delay, float speed);
    // Destructor
    ~APF();

    float processSample(float x, int channel);

    void setFs(float Fs);
    void setFeedbackGain(float feedbackGain);
    void setDepth(float depth);
    void setDelay(float delay)
    {
    	// 올-패스 필터에도 랜덤한 딜레이값을 주기 위해 사용
        fractionalDelay1.setDelaySamples(delay);
        fractionalDelay2.setDelaySamples(delay);
    }
    void setSpeed(float speed)
    {
        fractionalDelay1.setSpeed(speed);
        fractionalDelay2.setSpeed(speed);
    }

private:

    float Fs = 48000.f;

    // FD 생성
        // 중괄호 매개변수 {딜레이, 스피드} 전달
        // 샘플 길이는 소수, 스피드는 0.5f ~ 2.f 사이의 값
        // APF에서는 여러 개 필요없음
    FractionalDelay fractionalDelay1;
    FractionalDelay fractionalDelay2;

    // 피드백 경로 생성


    // 피드백 생성
    // 배열로 LR분리
    // APF는 경로 하나만 필요
    float fb1[2] = { 0.f }; //L,R 초기화
    float fb2[2] = { 0.f };

    // 피드백 게인 생성
    float feedbackGain1 = 0.5f;
    float feedbackGain2 = 0.5f;
    float depth = 5.0f; // percentage of intensity, control amp of LFO
    float channel;
	
    LPF lpf;

};



#endif /* APF_hpp */