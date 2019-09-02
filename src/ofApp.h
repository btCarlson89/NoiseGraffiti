#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "ColorPool.h"
#include "POP.h"

class ofApp : public ofBaseApp{

public:
    
    void setup();
    void update();
    void draw();
    
    void resetOffset();
    void resetPositions();
    
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
    
    void setNoiseScaleMin(float& value);
    void setNoiseScaleMax(float& value);
    void setNoiseStrengthMin(float& value);
    void setNoiseStrengthMax(float& value);

private:

    //  Window
    float width, height;
    
    //  POP
    POP* pop;
    
    //  Colors
    ColorPool intColors;
    ColorPool extColors;
    
    //  GUI
    ofxPanel gui;
    ofParameter<float> tf_noiseScaleMin;
    ofParameter<float> tf_noiseScaleMax;
    ofParameter<float> tf_noiseStrengthMin;
    ofParameter<float> tf_noiseStrengthMax;
};
