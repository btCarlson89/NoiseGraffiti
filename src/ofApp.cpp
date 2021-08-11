#include "ofApp.h"

//  Core Methods
////////////////////////////////////////////////////////////////
//--------------------------------------------------------------
void ofApp::setup(){

    //  OF Settings
    ofSetWindowShape(1080, 1080);
    //ofSetFullscreen(true);
    ofSetBackgroundAuto(false);
    ofSetCircleResolution(360);
    ofSetFrameRate(60);
    ofHideCursor();
    
    //  OSC
    oscIn.setup(10000);
    oscOut.setup("127.0.0.1", 9999);
    oscOut2.setup("127.0.0.1", 9998);
    mesOut.setAddress("/param1");
    
    //  Window
    width = ofGetWidth();
    height = ofGetHeight();
    
    //  FBOs
    fboPOP.allocate(width, height, GL_RGBA16F_ARB);
    fboPOP.begin();
    ofClear(0);
    fboPOP.end();
    
    //  Create Color Pools
    intColors.addColor(ofColor(  0,   0,   0,  255));
    intColors.addColor(ofColor(127, 127, 127,  255));
    intColors.addColor(ofColor(255, 255, 255,  255));
    intColors.addColor(ofColor(255, 255, 255,  255));
    intColors.addColor(ofColor(255, 255, 255,  255));
    
//    intColors.addColor(ofColor(255,  32,   64,  255));
//    intColors.addColor(ofColor(255,  32,   64,  255));
//    intColors.addColor(ofColor(255,  32,   64,  255));
    
    intColors.addColor(ofColor(  0,   0,   0,  255));
    intColors.addColor(ofColor(127, 127, 127,  255));
    intColors.addColor(ofColor(255, 255, 255,  255));
    intColors.addColor(ofColor(255, 255, 255,  255));
    intColors.addColor(ofColor(255, 255, 255,  255));
    
    extColors.addColor(ofColor(  0,   0,   0,  255));
    extColors.addColor(ofColor(255, 255, 255,  255));
    extColors.addColor(ofColor(  0,   0,   0,  255));
    extColors.addColor(ofColor(  0,   0,   0,  255));
    extColors.addColor(ofColor(255, 255, 255,  255));
    extColors.addColor(ofColor(  0,   0,   0,  255));
    
//    extColors.addColor(ofColor(255,  32,   64,  255));
//    extColors.addColor(ofColor(255,  32,   64,  255));
//    extColors.addColor(ofColor(255,  32,   64,  255));
    
    //  Init POP Nets
    pop = new POP(4096, width, height);
    pop->setInteriorColors(intColors);
    pop->setExteriorColors(extColors);
    
    gpop = new GPOP();
    gpop->setup(0.25 * width * height, width, height);
    initPOPPositions();
    initPOPVelocities();
    initPOPColors();
    
    //  GUI
    //  Add Listeners
    tf_noiseScaleMin.addListener(this, &ofApp::setNoiseScaleMin);
    tf_noiseScaleMax.addListener(this, &ofApp::setNoiseScaleMax);
    tf_noiseStrengthMin.addListener(this, &ofApp::setNoiseStrengthMin);
    tf_noiseStrengthMax.addListener(this, &ofApp::setNoiseStrengthMax);
    
    //  Setup Interface
    gui.setup("Parameters");
    gui.setPosition(20, 20);
    gui.add(tf_noiseScaleMin.set("Noise Scale Min", 1000.0f, 100.0f, 3000.0f));
    gui.add(tf_noiseScaleMax.set("Noise Scale Max", 1000.0f, 100.0f, 3000.0f));
    gui.add(tf_noiseStrengthMin.set("Noise Strength Min", 16.0f, 1.0f, 32.0f));
    gui.add(tf_noiseStrengthMax.set("Noise Strength Max", 16.0f, 1.0f, 32.0f));
    
    //  Post FX
    glitch1.load("shaders/glitch-01");
    glitch2.load("shaders/glitch-02");
    glitch3.load("shaders/glitch-03");
    glitch4.load("shaders/glitch-04");
    glitchLines.load("shaders/glitch-lines");
    
    glitch1Fbo.allocate(width, height, GL_RGBA16F_ARB);
    glitch1Fbo.begin();
    ofClear(0, 0, 0, 0);
    glitch1Fbo.end();
    
    glitch2Fbo.allocate(width, height, GL_RGBA16F_ARB);
    glitch2Fbo.begin();
    ofClear(0, 0, 0, 0);
    glitch2Fbo.end();
    
    glitch3Fbo.allocate(width, height, GL_RGBA16F_ARB);
    glitch3Fbo.begin();
    ofClear(0, 0, 0, 0);
    glitch3Fbo.end();
    
    glitch4Fbo.allocate(width, height, GL_RGBA16F_ARB);
    glitch4Fbo.begin();
    ofClear(0, 0, 0, 0);
    glitch4Fbo.end();
    
    glitchLinesFbo.allocate(width, height, GL_RGBA16F_ARB);
    glitchLinesFbo.begin();
    ofClear(0, 0, 0, 0);
    glitchLinesFbo.end();
    
    hatGlitchFbo.allocate(width, height, GL_RGBA16F_ARB);
    hatGlitchFbo.begin();
    ofClear(0, 0, 0, 0);
    hatGlitchFbo.end();
    
    plane.set(width, height, 100, 100);
    plane.mapTexCoords(0, 0, width, height);
    
    //  Test assets
    img.load("images/touch-2.png");
    video.load("climaxDanceOverhead01.mp4");
    video.play();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //  Post Fx Params
    glitch1Amp *= 0.93;
    glitch2Amp *= 0.92;
    glitch3Amp *= 0.92;
    glitch4Amp *= 0.97;
    hatGlitch  *= 0.97;
    
    lastChange += ofGetLastFrameTime();
    
    //  OSC In
    int note = 0;
    int vel  = 0;
    while(oscIn.hasWaitingMessages()){
        oscIn.getNextMessage(mesIn);
        if(mesIn.getAddress().rfind("/Note", 0) == 0){
            note = mesIn.getArgAsInt(0);
        }
        else if (mesIn.getAddress().rfind("/Velocity", 0) == 0){
            vel = mesIn.getArgAsInt(0);
        }
        
        if(note == 12 && vel > 10){
            dt = 0;
        }
        else if(note == 36 && vel > 10){    //  C - Kick
            glitch4Amp = 1.0f;
            gpop->updateBackground();
            
            ofBackground(0);
            fboPOP.begin();
            ofClear(0);
            fboPOP.end();
        }
        else if(note == 38 && vel > 10){    //  D
            glitch1Amp = 1.0f;
            if (lastChange > 0.25){
                video.setPosition(ofRandom(1));
                lastChange = 0;
            }
            //ofBackground(0);
            //fboPOP.begin();
            //ofClear(0);
            //fboPOP.end();
        }
        else if(note == 41 && vel > 10){    //  F
            glitch2Amp = 1.0f;
        }
        else if(note == 42 && vel > 10){    //  F#
            glitch3Amp = 1.0f;
        }
        else if(note == 45 && vel > 10){    //  A
            hatGlitch = 1.0f;
            hatPosX = ofRandom(width);
            hatPosY = ofRandom(height);
            hatW = ofRandom(0.005 * width, 0.1 * width);
        }
    }
    mesIn.clear();

    //  OSC Out
    mesOut.setAddress("/param1");
    mesOut.addFloatArg(dt / 20.0 + 0.15);
    if(mesOut.getNumArgs() > 0){
        oscOut.sendMessage(mesOut);
        oscOut2.sendMessage(mesOut);
        mesOut.clear();
    }
    
    //  Timeline
    if(dt == 0){
        resetColor();
        resetPositions();

        if(ofRandom(1.0f) > 0.5)
            resetOffset();
        else
            resetFrequency();
        
        ofBackground(0);
        fboPOP.begin();
        ofClear(0);
        fboPOP.end();
    }
    dt += 1.0f / ofGetFrameRate();
    
    //  POP Net Updates
    //pop->update();
    gpop->update();
    
    
    //  Window
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){

    //  BG
    ofClear(0);
    
    video.update();
    
    //  POP Net
    //pop->draw();
    
    fboPOP.begin();
    ofSetColor(0, 0, 0, 2);
    ofDrawRectangle(0, 0, width, height);
    gpop->draw();
    fboPOP.end();
    //ofSetColor(255, 255, 255);
    //fboPOP.draw(0, 0);
    
    //  Hat Glitch
    hatGlitchFbo.begin();
    ofSetColor(255, 255, 255);
    fboPOP.draw(0, 0);
    //img.draw(0, 0);
    //video.draw(0, 0);
    ofSetColor(255, 255, 255, 255 * hatGlitch);
    ofDrawRectangle(hatPosX, 0, hatW, height);
    hatGlitchFbo.end();
    
    //hatGlitchFbo.draw(0, 0);
    
    //  Glitch
    glitch1Fbo.begin();
    glitch1.begin();
    glitch1.setUniform1f("amp", glitch1Amp);
    glitch1.setUniform1f("time", ofGetElapsedTimef());
    glitch1.setUniform2f("res", width, height);
    glitch1.setUniformTexture("tex", hatGlitchFbo.getTexture(), 0);
    ofPushMatrix();
    ofTranslate(width/2, height/2);
    plane.draw();
    ofPopMatrix();
    glitch1.end();
    glitch1Fbo.end();
    
    //  Glitch
    glitch2Fbo.begin();
    glitch2.begin();
    glitch2.setUniform1f("amp", glitch2Amp);
    glitch2.setUniform1f("time", ofGetElapsedTimef());
    glitch2.setUniform2f("res", width, height);
    glitch2.setUniformTexture("tex", glitch1Fbo.getTexture(), 0);
    ofPushMatrix();
    ofTranslate(width/2, height/2);
    plane.draw();
    ofPopMatrix();
    glitch2.end();
    glitch2Fbo.end();
    
    //  Glitch
    glitch3Fbo.begin();
    glitch3.begin();
    glitch3.setUniform1f("amp", glitch3Amp);
    glitch3.setUniform1f("time", ofGetElapsedTimef());
    glitch3.setUniform2f("res", width, height);
    glitch3.setUniformTexture("tex", glitch2Fbo.getTexture(), 0);
    ofPushMatrix();
    ofTranslate(width/2, height/2);
    plane.draw();
    ofPopMatrix();
    glitch3.end();
    glitch3Fbo.end();
    
    //  Glitch
    glitch4Fbo.begin();
    glitch4.begin();
    glitch4.setUniform1f("amp", glitch4Amp);
    glitch4.setUniform1f("time", ofGetElapsedTimef());
    glitch4.setUniform2f("res", width, height);
    glitch4.setUniformTexture("tex", glitch3Fbo.getTexture(), 0);
    ofPushMatrix();
    ofTranslate(width/2, height/2);
    plane.draw();
    ofPopMatrix();
    glitch4.end();
    glitch4Fbo.end();
    
    //  Glitch Lines
    glitchLinesFbo.begin();
    glitchLines.begin();
    glitchLines.setUniform2f("res", width, height);
    glitchLines.setUniform1f("time", ofGetElapsedTimef());
    glitchLines.setUniform1f("low", glitch1Amp);
    glitchLines.setUniform1f("divs", ofRandom(32));
    glitchLines.setUniformTexture("tex", glitch4Fbo.getTexture(), 0);
    ofPushMatrix();
    ofTranslate(width/2, height/2);
    plane.draw();
    ofPopMatrix();
    glitchLines.end();
    glitchLinesFbo.end();
    
    //glitch1Fbo.draw(0, 0);
    //glitch4Fbo.draw(0, 0);
    glitchLinesFbo.draw(0, 0);
    
    
    //  GUI
    //gui.draw();
}

//  Resets
////////////////////////////////////////////////////////////////
//--------------------------------------------------------------
void ofApp::resetOffset(){
    //  CPOP Method
    pop->resetOffset();
    
    //  GPOP Method
    glm::vec3 offset = glm::vec3(ofRandom(-10000, 10000), ofRandom(-10000, 10000), ofRandom(-10000, 10000));
    gpop->setNoiseOffset(offset);
}

//--------------------------------------------------------------
void ofApp::resetPositions(){
    //  CPOP Method
    pop->resetPositions();
    
    //  GPOP Method
    float* data = new float[(int)width * (int)height * 4];
    for (unsigned y = 0; y < height; ++y)
    {
        for (unsigned x = 0; x < width; ++x)
        {
            float angle = ofRandom(TWO_PI);
            float radius = ofRandom(0.4 * height);
            unsigned idx = y * width + x;
            data[idx * 4 + 0] = ofRandom(width);
            data[idx * 4 + 1] = ofRandom(height);
            data[idx * 4 + 2] = ofRandom(-4, 4);
            data[idx * 4 + 3] = 1.0f;
        }
    }
    gpop->loadDataTexture(GPOP::POSITION, data);
    delete[] data;
}

//--------------------------------------------------------------
void ofApp::resetFrequency(){
    float frequencyMin = ofRandom(1, 32);
    float frequencyMax = ofRandom(1, 8);
    
    //  CPOP Method
    
    //  GPOP Method
    gpop->setNoiseFrequencyMin(frequencyMin);
    gpop->setNoiseFrequencyMax(frequencyMax);
}

//--------------------------------------------------------------
void ofApp::resetScale(){
    float scale = ofRandom(400.0f, 2400.0f);
    //  CPOP Method
    
    //  GPOP Method
    gpop->setNoiseScaleMin(scale);
    gpop->setNoiseScaleMax(scale);
}

//--------------------------------------------------------------
void ofApp::resetColor(){
    //  GPOP Method
    int intIdx = ofRandom(gpop->getNumInteriorColors());
    gpop->setIntColorIndex(intIdx);
    
    int extIdx = ofRandom(gpop->getNumExteriorColors());
    gpop->setExtColorIndex(extIdx);
}

//  Events
////////////////////////////////////////////////////////////////
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case 'b':
            gpop->updateBackground();
            break;
        case 'c':
            ofBackground(0);
            fboPOP.begin();
            ofClear(0);
            fboPOP.end();
            break;
        case 'f':
            resetFrequency();
            break;
        case 'h':
            resetColor();
            break;
        case 'o':
            resetOffset();
            break;
        case 'p':
            resetPositions();
            break;
        case 's':
            resetScale();
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

//  Interface signals
////////////////////////////////////////////////////////////////
//--------------------------------------------------------------
void ofApp::setNoiseScaleMin(float& value){
    pop->setNoiseScaleMin(value);
    
    gpop->setNoiseScaleMin(value);
}

//--------------------------------------------------------------
void ofApp::setNoiseScaleMax(float& value){
    pop->setNoiseScaleMax(value);
    
    gpop->setNoiseScaleMax(value);
}

//--------------------------------------------------------------
void ofApp::setNoiseStrengthMin(float& value){
    pop->setNoiseStrengthMin(value);
    
    gpop->setNoiseFrequencyMin(value);
}

//--------------------------------------------------------------
void ofApp::setNoiseStrengthMax(float& value){
    pop->setNoiseStrengthMax(value);
    
    gpop->setNoiseFrequencyMax(value);
}

//  POP Net operations
////////////////////////////////////////////////////////////////
//--------------------------------------------------------------
void ofApp::initPOPPositions(){
    //  GPOP Method
    float* data = new float[(int)width * (int)height * 4];
    for (unsigned y = 0; y < height; ++y)
    {
        for (unsigned x = 0; x < width; ++x)
        {
            unsigned idx = y * width + x;
            data[idx * 4 + 0] = ofRandom(0, width);
            data[idx * 4 + 1] = ofRandom(0, height);
            data[idx * 4 + 2] = ofRandom(-0.4, 0.4);
            data[idx * 4 + 3] = 1.0f;
        }
    }
    gpop->loadDataTexture(GPOP::POSITION, data);
    delete[] data;
}

//--------------------------------------------------------------
void ofApp::initPOPVelocities(){
    //  GPOP Method
    float* data = new float[(int)width * (int)height * 4];
    for (unsigned y = 0; y < height; ++y)
    {
        for (unsigned x = 0; x < width; ++x)
        {
            float stepSize = ofRandom(0.1, 1.0);
            unsigned idx = y * width + x;
            data[idx * 4 + 0] = stepSize;
            data[idx * 4 + 1] = stepSize;
            data[idx * 4 + 2] = stepSize;
            data[idx * 4 + 3] = 0.0f;
        }
    }
    gpop->loadDataTexture(GPOP::VELOCITY, data);
    delete[] data;
}

//--------------------------------------------------------------
void ofApp::initPOPColors(){
    //  GPOP Method - Init texture
    float* data = new float[(int)width * (int)height * 4];
    for (unsigned y = 0; y < height; ++y)
    {
        for (unsigned x = 0; x < width; ++x)
        {
            unsigned idx = y * width + x;
            data[idx * 4 + 0] = 1.0f;
            data[idx * 4 + 1] = 1.0f;
            data[idx * 4 + 2] = 1.0f;
            data[idx * 4 + 3] = 0.01f;
        }
    }
    gpop->loadDataTexture(GPOP::COLOR, data);
    delete[] data;
    
    //  Color Pools
    gpop->setInteriorColors(intColors);
    gpop->setExteriorColors(extColors);
}

/*
 intColors.addColor(ofColor(216,  17,  89,   4));
 intColors.addColor(ofColor(143,  45,  86,   4));
 intColors.addColor(ofColor( 33, 131, 128,   4));
 intColors.addColor(ofColor(251, 177,  60,   4));
 intColors.addColor(ofColor(115, 210, 222,   4));
 intColors.addColor(ofColor(  0,   0,   0, 255));
 
 intColors.addColor(ofColor(  4, 231,  98,   4));
 intColors.addColor(ofColor(245, 183,   0,   4));
 intColors.addColor(ofColor(220,   0, 115,   4));
 intColors.addColor(ofColor(  0, 139, 248,   4));
 intColors.addColor(ofColor(137, 252,   0,   4));
 intColors.addColor(ofColor(  0,   0,   0, 255));
 */
