//
//  FDN.hpp
//
//  Created by Eric Tarr on 2/6/20.
//  Copyright © 2020 Eric Tarr. All rights reserved.
//

#ifndef FDN_hpp
#define FDN_hpp

#include "FractionalDelay.hpp" // FDN 내부에서 Fractional Delay 블록을 사용해야하기 때문에

using namespace std;

class FDN {

public:

    // Constructor function (special function - no return type, name = Class name)
    FDN();

    // Destructor
    ~FDN();

    float processSample(float x, int channel);

    void setFs(float Fs);
    void setTime(float timeValue);
    void setDepth(float depth);


private:

    float Fs = 48000.f;

    // FD 생성
        // 중괄호 매개변수 {딜레이, 스피드} 전달
        // 샘플 길이는 소수, 스피드는 0.5f ~ 2.f 사이의 값

    FractionalDelay fractionalDelay1{ 1571.f, 0.7f };
    FractionalDelay fractionalDelay2{ 2053.f, 0.9f };
    FractionalDelay fractionalDelay3{ 2719.f, 1.1f };
    FractionalDelay fractionalDelay4{ 3463.f, 1.3f };

    // 피드백 경로 생성


    // 피드백 생성
    // 배열로 LR분리
    float fb1[2] = { 0.f };
    float fb2[2] = { 0.f };
    float fb3[2] = { 0.f };
    float fb4[2] = { 0.f };

    // 피드백 게인 생성
    float feedbackGain = 0.5f;


    float depth = 10.0f; // percentage of intensity, control amp of LFO



};



#endif /* FDN_hpp */