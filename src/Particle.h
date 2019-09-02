//
//  Particle.hpp
//  noisePaint
//
//  Created by Brian Carlson on 8/30/19.
//

#pragma once

#include "ofMain.h"

class POP;

class Particle {
  
public:
    
    //  Constructors
    ////////////////
    
    Particle(POP* parent);
    
    //  Core methods
    ////////////////
    
    void update();
    
    void draw();
    
//private:
    
    glm::vec3 pos;
    glm::vec3 origin;
    
    float width, height;
    float noiseScale;
    float noiseStrength;
    float noiseZ;
    float dNoiseZ;
    float stepSize;
    float angle;
    float radius;
    
    POP* parent;
};
