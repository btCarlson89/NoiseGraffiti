#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    //  OF Settings
    ofSetBackgroundAuto(false);
    ofBackground(0);
    ofSetCircleResolution(360);
    ofSetFrameRate(60);
    
    //  Window
    width = ofGetWidth();
    height = ofGetHeight();
    
    //  POP Net
    pop = new POP(4096, width, height);
    
    //  Colors
    intColors.addColor(ofColor(216,  17,  89));
    intColors.addColor(ofColor(143,  45,  86));
    intColors.addColor(ofColor( 33, 131, 128));
    intColors.addColor(ofColor(251, 177,  60));
    intColors.addColor(ofColor(115, 210, 222));
    
    extColors.addColor(ofColor(  7,   7,   7));
    extColors.addColor(ofColor( 15,  15,  15));
    extColors.addColor(ofColor( 31,  31,  31));
    extColors.addColor(ofColor( 63,  63,  63));
    extColors.addColor(ofColor(127, 127, 127));
    
    pop->setInteriorColors(intColors);
    pop->setExteriorColors(extColors);
    
    //  GUI
    //  Listeners
    tf_noiseScaleMin.addListener(this, &ofApp::setNoiseScaleMin);
    tf_noiseScaleMax.addListener(this, &ofApp::setNoiseScaleMax);
    tf_noiseStrengthMin.addListener(this, &ofApp::setNoiseStrengthMin);
    tf_noiseStrengthMax.addListener(this, &ofApp::setNoiseStrengthMax);
    
    //  Interface
    gui.setup("Parameters");
    gui.setPosition(20, 20);
    gui.add(tf_noiseScaleMin.set("Noise Scale Min", 300.0f, 1.0f, 2000.0f));
    gui.add(tf_noiseScaleMax.set("Noise Scale Max", 500.0f, 1.0f, 2000.0f));
    gui.add(tf_noiseStrengthMin.set("Noise Strength Min", 1.0f, 1.0f, 20.0f));
    gui.add(tf_noiseStrengthMax.set("Noise Strength Max", 1.0f, 1.0f, 20.0f));
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //  POP Net
    pop->update();
    
    //  Window
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){

    //  POP Net
    pop->draw();
    
    //  GUI
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::resetOffset(){
    pop->resetOffset();
}

//--------------------------------------------------------------
void ofApp::resetPositions(){
    pop->resetPositions();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case 'c':
            ofBackground(0);
            resetOffset();
            break;
        case 'r':
            resetPositions();
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::setNoiseScaleMin(float& value){
    pop->setNoiseScaleMin(value);
}

//--------------------------------------------------------------
void ofApp::setNoiseScaleMax(float& value){
    pop->setNoiseScaleMax(value);
}

//--------------------------------------------------------------
void ofApp::setNoiseStrengthMin(float& value){
    pop->setNoiseStrengthMin(value);
}

//--------------------------------------------------------------
void ofApp::setNoiseStrengthMax(float& value){
    pop->setNoiseStrengthMax(value);
}
