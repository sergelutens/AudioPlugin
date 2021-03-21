//
//  LPF.hpp
//
//  Created by Eric Tarr on 2/6/20.
//  Copyright © 2020 Eric Tarr. All rights reserved.
//

#ifndef LPF_hpp
#define LPF_hpp

class LPF {

public:


    LPF();

    ~LPF();

    float processSample(float x, int channel);

    void setFs(float Fs);
    void setFrequency(float newFreq);


private:

    float Fs = 48000.f;
    float freq = 20000.f;

    const float Q = 0.7071f;
	
    float x1[2] = { 0.f };
    float x2[2] = { 0.f };
    float y1[2] = { 0.f };
    float y2[2] = { 0.f };

    float b0 = 1.f; float b1 = 0.f; float b2 = 0.f;
    float a0 = 1.f; float a1 = 0.f; float a2 = 0.f;

    void updateCoefficients();
};



#endif /* LPF_hpp */