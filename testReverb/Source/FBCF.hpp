//
//  FBCF.hpp
//
//  Created by Eric Tarr on 2/6/20.
//  Copyright © 2020 Eric Tarr. All rights reserved.
//

#ifndef FBCF_hpp
#define FBCF_hpp

#include "FractionalDelay.hpp" // FBCF 내부에서 Fractional Delay 블록을 사용해야하기 때문에
#include "APF.hpp" // 올 패스 필터를 피드백시키기 위해 포함시킴 
using namespace std;

class FBCF {

public:

    // Constructor function (special function - no return type, name = Class name)
    FBCF();

    // FBCF의 딜레이 값과 스피드를 만들어 줌
	// Exponential 다양화를 위해 콤필터 마다 올 패스 필터 값 부여
    FBCF(float delay, float speed);
    FBCF(float delay, float speed, float apfDelay, float apfSpeed);
    // Destructor
    ~FBCF();

    float processSample(float x, int channel);

    void setFs(float Fs);
    void setFeedbackGain(float feedbackGain);
    void setDepth(float depth);


private:
	
    float Fs = 48000.f;

    // FD 생성
        // 중괄호 매개변수 {딜레이, 스피드} 전달
        // 샘플 길이는 소수, 스피드는 0.5f ~ 2.f 사이의 값
        // FBCF에서는 각 FBCF를 모두 변조해야 함
    FractionalDelay fractionalDelay{ 240.f, 0.7f };


    // 피드백 경로 생성


    // 피드백 생성
    // 배열로 LR분리
    // FBCF는 경로 하나만 필요
    float fb1[2] = { 0.f }; //L,R 초기화


    // 피드백 게인 생성
    float feedbackGain = 0.5f;

    float depth = 10.0f; // percentage of intensity, control amp of LFO

	
    class SimpleLPF
    {
    public:
        float processSample(float x, int channel)
	    {
		    // 파일로 구현하지 말고 여기서 바로 구현하자.
        	// 딜레이가 반복될 때, 20khz부근 고역대에서 3db가 제거됨
        	// 리버브가 감소될 수록 고역대의 소리가 감소
        	float y = 0.5f * x + 0.5f * ff[channel];
            ff[channel] = x; //x를 다시 업데이트
            return y;
	    }
    private:
        float ff[2] = { 0.f }; // 피드 포워드 샘플 채널 당 0.f로 초기화
    };

    SimpleLPF lpf;
    APF apf;
};



#endif /* FBCF_hpp */