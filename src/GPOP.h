//
//  POP.h
//  ParticleSceneRnD
//
//  Created by Brian on 6/15/19.
//

#pragma once

#include "ofMain.h"
#include "ColorPool.h"
#include "PingPong.h"

class GPOP
{
    
public:
    
    //  Data for improving code readibility
    ///////////////////////////////////////
    
    static const unsigned FLOATS_PER_TEXEL = 4;
    
    enum DataTextureIndex
    {
        POSITION,
        VELOCITY,
        COLOR,
        NUM_TEXTURES
    };
    
    //  Constructors
    ////////////////
    
    GPOP();
    
    //  Core methods
    ////////////////
    
    void setup(unsigned particleCount, float w, float h);
    
    void update();
    
    void draw();
    
    void updateBackground();
    
    //  Data handling
    /////////////////
    
    void loadDataTexture(unsigned idx, float* data,
                         unsigned x = 0, unsigned y = 0, unsigned width = 0, unsigned height = 0);
    
    void zeroDataTexture(unsigned idx,
                         unsigned x = 0, unsigned y = 0, unsigned width = 0, unsigned height = 0);
    
    //  Color set functions
    ///////////////////////
    
    void setInteriorColors(ColorPool pool);
    
    void setExteriorColors(ColorPool pool);
    
    int getNumInteriorColors();
    
    int getNumExteriorColors();
    
    void setIntColorIndex(int idx);
    
    void setExtColorIndex(int idx);
    
    void randomizeColor();
    
    //  Shading
    ///////////
        
    void loadShaders(const string& updateShaderName, const string& drawShaderName);
    
    //  Buffer & texture handling
    /////////////////////////////
    
    ofFbo::Settings getFboSettings(unsigned numColorBuffers = 1) const;
    
    ofTextureData getTextureSettings() const;
    
    //  Member get & set functions
    void setRadius(float& value);
    
    float getRadius();
    
    void setNoiseScaleMin(float& value);
    
    float getNoiseScaleMin();
    
    void setNoiseScaleMax(float& value);
    
    float getNoiseScaleMax();
    
    void setNoiseFrequencyMin(float value);
    
    float getNoiseFrequencyMin();
    
    void setNoiseFrequencyMax(float value);
    
    float getNoiseFrequencyMax();
    
    void setNoiseOffset(glm::vec3& offset);
    
    glm::vec3 getNoiseOffset();
    
private:
    
    //  Simulation
    //////////////
    
    unsigned count;
    
    float noiseScaleMin, noiseScaleMax;
    
    float noiseFrequencyMin, noiseFrequencyMax;
    
    float dZ;
    
    glm::vec3 origin;
    
    glm::vec3 offset;
    
    float radius;
    
    ofImage bgImg;
    
    //  Data Textures
    /////////////////
    
    PingPong pp;
    
    unsigned textureLocation;
    
    unsigned width, height, numFloats;
    
    //  Data Meshes
    ///////////////
    
    ofVboMesh mesh;
    
    //  Colors
    //////////
    
    ColorPool intColors;
    
    int intColorIdx = 0;
    
    ColorPool extColors;
    
    int extColorIdx = 0;
    
    //  Shading
    ///////////
    
    ofShader updateShader;
    ofShader drawShader;
    ofShader bg;
    
    ofFbo bgFbo;
    
    ofPlanePrimitive plane;
    
    //  Window
    //////////
    
    float screenW, screenH;
};

