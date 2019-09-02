//
//  ColorPool.hpp
//  noisePaint
//
//  Created by Brian Carlson on 8/30/19.
//

#pragma once

#include "ofMain.h"

class ColorPool {
public:
    
    ColorPool();
    
    void addColor(ofColor color);
    void addColors(vector<ofColor> colors);
    
    ofColor getColor(int idx);
    ofColor getRandomColor();
    
    int numColors();
    
private:
    
    vector<ofColor> pool;
};
