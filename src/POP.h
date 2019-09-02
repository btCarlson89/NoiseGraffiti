//
//  POP.hpp
//  noisePaint
//
//  Created by Brian Carlson on 8/30/19.
//

#pragma once

#include "ofMain.h"

#include "ColorPool.h"
#include "Particle.h"

class POP {
  
public:
    
    //  Constructors
    ////////////////
    
    POP();
    
    POP(int count, float w, float h);
    
    //  Core methods
    ////////////////
    
    void update();
    
    void draw();
    
    void resetOffset();
    
    void resetPositions();
    
    //  Color set functions
    ///////////////////////
    
    void setInteriorColors(ColorPool pool);
    
    void setExteriorColors(ColorPool pool);
    
    //  Parameter set & get functions
    /////////////////////////////////
    float getWidth();
    
    float getHeight();
    
    glm::vec3 getOffset();
    
    void setNoiseScaleMin(float& value);
    
    float getNoiseScaleMin();
    
    void setNoiseScaleMax(float& value);
    
    float getNoiseScaleMax();
    
    void setNoiseStrengthMin(float& value);
    
    float getNoiseStrengthMin();
    
    void setNoiseStrengthMax(float& value);
    
    float getNoiseStrengthMax();
    
    
private:
    
    //  Base config
    ///////////////
    
    int count;
    
    int radius;
    
    glm::vec3 origin;
    
    glm::vec3 offset;
    
    float width, height;
    
    //  Particles
    /////////////
    
    vector<Particle> particles;
    
    //  Colors
    //////////
    
    ColorPool intColors;
    
    int intColorIdx = 0;
    
    ColorPool extColors;
    
    int extColorIdx = 0;
    
    float dt = 0.0f;
    
    //  Parameters
    //////////////
    
    float noiseScaleMin = 1.0f;
    float noiseScaleMax = 1000.0f;
    float noiseStrengthMin = 1.0f;
    float noiseStrengthMax = 10.0f;
};
