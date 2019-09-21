//
//  PingPong.cpp
//  fluidStudy
//
//  Created by Brian on 3/8/19.
//

#include "PingPong.h"

PingPong::PingPong(){}

void PingPong::allocate(int w, int h, int internalFormat) {
    //  Allocate
    for (int i = 0; i < 2; ++i) {
        fbos[i].allocate(w, h, internalFormat);
    }
    
    //  Clear
    clear();
}

void PingPong::swap() {
    //  Swap
    std::swap(src, dst);
}

void PingPong::clear() {
    //  Clear
    for (int i = 0; i < 2; ++i) {
        fbos[i].begin();
        ofClear(0, 0, 0, 0);
        fbos[i].end();
    }
}
