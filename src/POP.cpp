//
//  POP.cpp
//  noisePaint
//
//  Created by Brian Carlson on 8/30/19.
//

#include "POP.h"

////////////////////////////////////////////////////////////////
POP::POP(){
    count = 0;
    radius = 100;
    width = ofGetWidth();
    height = ofGetHeight();
}

////////////////////////////////////////////////////////////////
POP::POP(int count, float w, float h){
    this->count = count;
    width = w;
    height = h;
    radius = min(width, height) * 0.4;
    
    origin = glm::vec3(width / 2, height / 2, 0);
    offset = glm::vec3(ofRandom(-width, width), ofRandom(-height, height), ofRandom(-100, 100));
    
    particles.clear();
    for(int i = 0; i < count; ++i){
        particles.push_back(Particle(this));
    }
}

////////////////////////////////////////////////////////////////
void POP::update(){
    for(auto& particle : particles){
        particle.update();
    }
    
    dt += 1.0f / ofGetFrameRate();
}

////////////////////////////////////////////////////////////////
void POP::draw(){
    for(auto particle : particles){
        float xpos = particle.pos.x - origin.x;
        float ypos = particle.pos.y - origin.y;
        float zpos = particle.noiseZ;
        float alpha;
        float length = sqrt(xpos*xpos + ypos*ypos + zpos*zpos);
        
        bool inside = false;
        
        if(length < radius)
            inside = true;
        else {
            alpha = ofMap(length, 0.333 * height, width / 2, 4, 32, true);
        }
        
        if(dt > 4.0){
            intColorIdx = ofRandom(intColors.numColors());
            extColorIdx = ofRandom(extColors.numColors());
            dt = 0.0f;
        }
            
        if(inside){
            if(intColors.numColors() > 0)
                ofSetColor(intColors.getColor(intColorIdx), 32);
            else
                ofSetColor(255, 32);
        }
        else{
            if(extColors.numColors() > 0)
                ofSetColor(extColors.getColor(extColorIdx), alpha);
            else
                ofSetColor(255, alpha);
        }
        
        particle.draw();
    }
}

////////////////////////////////////////////////////////////////
void POP::resetOffset(){
    offset = glm::vec3(ofRandom(-width, width), ofRandom(-height, height), ofRandom(-0.01, .01));
}

////////////////////////////////////////////////////////////////
void POP::resetPositions(){
    float xpos = width / 2;
    float ypos = height / 2;
    float angle;
    float radius;
    
    for(auto& particle : particles){
        angle = ofRandom(TWO_PI);
        radius = 0.39* height;
        
        particle.pos = glm::vec3(xpos + radius * cos(angle), ypos + radius * sin(angle), ofRandom(-0.01, 0.01));
    }
}

////////////////////////////////////////////////////////////////
void POP::setInteriorColors(ColorPool pool){
    intColors = pool;
}

////////////////////////////////////////////////////////////////
void POP::setExteriorColors(ColorPool pool){
    extColors = pool;
}

////////////////////////////////////////////////////////////////
float POP::getWidth(){
    return width;
}

////////////////////////////////////////////////////////////////
float POP::getHeight(){
    return height;
}

////////////////////////////////////////////////////////////////
glm::vec3 POP::getOffset(){
    return offset;
}

////////////////////////////////////////////////////////////////
void POP::setNoiseScaleMin(float& value){
    noiseScaleMin = value;
}

////////////////////////////////////////////////////////////////
float POP::getNoiseScaleMin(){
    return noiseScaleMin;
}

////////////////////////////////////////////////////////////////
void POP::setNoiseScaleMax(float& value){
    noiseScaleMax = value;
}

////////////////////////////////////////////////////////////////
float POP::getNoiseScaleMax(){
    return noiseScaleMax;
}

////////////////////////////////////////////////////////////////
void POP::setNoiseStrengthMin(float& value){
    noiseStrengthMin = value;
}

////////////////////////////////////////////////////////////////
float POP::getNoiseStrengthMin(){
    return noiseStrengthMin;
}

////////////////////////////////////////////////////////////////
void POP::setNoiseStrengthMax(float& value){
    noiseStrengthMax = value;
}

////////////////////////////////////////////////////////////////
float POP::getNoiseStrengthMax(){
    return noiseStrengthMax;
}
