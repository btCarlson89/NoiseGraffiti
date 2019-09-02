//
//  ColorPool.cpp
//  noisePaint
//
//  Created by Brian Carlson on 8/30/19.
//

#include "ColorPool.h"

ColorPool::ColorPool(){}

void ColorPool::addColor(ofColor color){
    pool.push_back(color);
}

void ColorPool::addColors(vector<ofColor> colors){
    for(auto color : colors){
        pool.push_back(color);
    }
}

ofColor ColorPool::getColor(int idx){
    return pool.at(idx % pool.size());
}

ofColor ColorPool::getRandomColor(){
    int idx = ofRandom(pool.size());
    return pool.at(idx);
}

int ColorPool::numColors(){
    return (int) pool.size();
}
