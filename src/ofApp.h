#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOsc.h"

#include "ColorPool.h"
#include "GPOP.h"
#include "POP.h"

class ofApp : public ofBaseApp{

public:
    
    //  Core methods
    ////////////////
    
    void setup();
    void update();
    void draw();
    
    //  Events
    //////////
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    //  Resets
    //////////
    
    void resetOffset();
    void resetPositions();
    void resetFrequency();
    void resetScale();
    void resetColor();
    
    //  Interface signals
    /////////////////////
    void setNoiseScaleMin(float& value);
    void setNoiseScaleMax(float& value);
    void setNoiseStrengthMin(float& value);
    void setNoiseStrengthMax(float& value);

private:

    //  Window
    //////////
    
    float width, height;
    
    //  Timeline
    float dt = 0.0f;
    
    //  GUI
    ///////
    
    ofxPanel gui;
    ofParameter<float> tf_noiseScaleMin;
    ofParameter<float> tf_noiseScaleMax;
    ofParameter<float> tf_noiseStrengthMin;
    ofParameter<float> tf_noiseStrengthMax;
    
    //  OSC
    ///////
    ofxOscReceiver oscIn;
    ofxOscMessage mesIn;
    ofxOscSender oscOut;
    ofxOscSender oscOut2;
    ofxOscMessage mesOut;
    
    //  POP Nets
    ////////////
    
    POP* pop;
    GPOP* gpop;
    
    void initPOPPositions();
    void initPOPVelocities();
    void initPOPColors();
    
    //  Colors
    //////////
    
    ColorPool intColors;
    ColorPool extColors;
    
    //  Draw Buffers
    ofFbo fboPOP;
    
    //  Post FX
    ofShader glitch1;
    ofFbo glitch1Fbo;
    float glitch1Amp = 0.0f;
    
    ofShader glitch2;
    ofFbo glitch2Fbo;
    float glitch2Amp = 0.0f;
    
    ofShader glitch3;
    ofFbo glitch3Fbo;
    float glitch3Amp = 0.0f;
    
    ofShader glitch4;
    ofFbo glitch4Fbo;
    float glitch4Amp = 0.0f;
    
    ofFbo hatGlitchFbo;
    float hatGlitch = 0.0f;
    int hatPosX;
    int hatPosY;
    int hatW;
    
    ofPlanePrimitive plane;
};
