//
//  PingPong.h
//  fluidStudy
//
//  Created by Brian on 3/8/19.
//

#pragma once

#include "ofMain.h"

class PingPong
{
public:
    
    PingPong();
    
    void allocate(int w, int h, int internalFormat = GL_RGBA32F_ARB);
    void swap();
    void clear();
    
    ofFbo& operator[](int n) { return fbos[n]; }
    
    ofFbo* src = &fbos[0];
    ofFbo* dst = &fbos[1];
    
private:
    
    ofFbo fbos[2];
};
