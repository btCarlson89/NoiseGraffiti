#include "GPOP.h"

////////////////////////////////////////////////////////////////
GPOP::GPOP()
{
    textureLocation = 0;
    width = 0;
    height = 0;
    
    count = 0;
    
    noiseScaleMin = 1600.0f;
    noiseScaleMax = 1600.0f;
    noiseFrequencyMin = 24.0f;
    noiseFrequencyMax = 24.0f;
    dZ = 0.01f;
    
    screenW = ofGetWidth();
    screenH = ofGetHeight();
}

////////////////////////////////////////////////////////////////
void GPOP::setup(unsigned particleCount, float w, float h)
{
    //  Simulation data
    count = particleCount;
    bgImg.load("images/bg-01.png");
    
    //  Data texture dims
    width = sqrt(particleCount);
    height = width;
    numFloats = width * height * FLOATS_PER_TEXEL;
    
    //  Screen dims
    screenW = w;
    screenH = h;
    
    radius = 0.4 * min(screenW, screenH);
    origin = glm::vec3(screenW / 2, screenH / 2, 0.0);
    offset = glm::vec3(ofRandom(-10000, 10000), ofRandom(-10000, 10000), ofRandom(-10000, 10000));
    
    //  Fbo allocation
    ofFbo::Settings s = getFboSettings(NUM_TEXTURES);
    pp.src->allocate(s);
    pp.dst->allocate(s);
    bgFbo.allocate(screenW, screenH, GL_RGBA, 4);
    bgFbo.begin();
    ofClear(0);
    bgFbo.end();
    
    //  Meshes
    mesh.clear();
    for (int y = 0; y < height; ++y){
        for (int x = 0; x < width; ++x){
            mesh.addVertex(glm::vec3(0.0f, 0.0f, 0.0f));
            mesh.addTexCoord(glm::vec2(x, y));
        }
    }
    mesh.setMode(OF_PRIMITIVE_POINTS);
    
    //  zero initial positions
    zeroDataTexture(GPOP::POSITION);
    
    //  zero initial velocities
    zeroDataTexture(GPOP::VELOCITY);
    
    //  zero initial colors
    zeroDataTexture(GPOP::COLOR);
    
    //  shaders
    updateShader.load("shaders/update");
    drawShader.load("shaders/draw");
    bg.load("shaders/bg-01");
    
    plane.set(2, 2);
    plane.mapTexCoords(0, 0, width, height);
    
    updateBackground();
}

////////////////////////////////////////////////////////////////
void GPOP::update()
{
    pp.dst->begin(OF_FBOMODE_NODEFAULTS);
    pp.dst->activateAllDrawBuffers();
    
    ofPushStyle();
    glViewport(0, 0, width, height);
    ofDisableBlendMode();
    ofSetColor(255, 255, 255);
    updateShader.begin();
    updateShader.setUniform1f("elapsed", ofGetLastFrameTime());
    updateShader.setUniform1f("time", ofGetElapsedTimef());
    updateShader.setUniform2f("res", screenW, screenH);
    updateShader.setUniform1f("noiseScaleMin", noiseScaleMin);
    updateShader.setUniform1f("noiseScaleMax", noiseScaleMax);
    updateShader.setUniform1f("noiseFrequencyMin", noiseFrequencyMin);
    updateShader.setUniform1f("noiseFrequencyMax", noiseFrequencyMax);
    updateShader.setUniform1f("dZ", dZ);
    updateShader.setUniform4f("intColor", ofFloatColor(intColors.getColor(intColorIdx)));
    updateShader.setUniform4f("extColor", ofFloatColor(extColors.getColor(extColorIdx)));
    updateShader.setUniform1f("radius", radius);
    updateShader.setUniform3f("origin", origin);
    updateShader.setUniform3f("offset", offset);
    updateShader.setUniformTexture("inPos", pp.src->getTexture(POSITION), POSITION);
    updateShader.setUniformTexture("inVel", pp.src->getTexture(VELOCITY), VELOCITY);
    updateShader.setUniformTexture("inCol", pp.src->getTexture(COLOR), COLOR);
    updateShader.setUniformTexture("bg", bgFbo.getTexture(), 3);
    plane.draw();
    updateShader.end();
    
    ofPopStyle();
    
    pp.dst->end();
    pp.swap();
}

////////////////////////////////////////////////////////////////
void GPOP::draw()
{
    glPointSize(24);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    drawShader.begin();
    drawShader.setUniformTexture("inPos", pp.src->getTexture(POSITION), POSITION);
    drawShader.setUniformTexture("inVel", pp.src->getTexture(VELOCITY), VELOCITY);
    drawShader.setUniformTexture("inCol", pp.src->getTexture(COLOR), COLOR);
    mesh.draw();
    drawShader.end();
    ofEnableBlendMode(OF_BLENDMODE_DISABLED);
    
    //bgFbo.draw(0, 0);
}

////////////////////////////////////////////////////////////////
void GPOP::updateBackground(){
    bgFbo.begin();
    bg.begin();
    bg.setUniform1f("time", ofGetElapsedTimef());
    bg.setUniform2f("res", screenW, screenH);
    ofSetColor(255);
    ofDrawRectangle(-screenW * 0.5, -screenH * 0.5, screenW, screenH);
    bg.end();
    bgFbo.end();
}

////////////////////////////////////////////////////////////////
void GPOP::loadDataTexture(unsigned idx, float* data,
                          unsigned x, unsigned y, unsigned width, unsigned height)
{
    if (idx < pp.src->getNumTextures())
    {
        if (!width) width = this->width;
        if (!height) height = this->height;
        pp.src->getTexture(idx).bind();
        glTexSubImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, x, y, width, height, GL_RGBA, GL_FLOAT, data);
        pp.src->getTexture(idx).unbind();
    }
    else ofLogError() << "Trying to load data from array into non-existent buffer.";
}

////////////////////////////////////////////////////////////////
void GPOP::zeroDataTexture(unsigned idx,
                          unsigned x, unsigned y, unsigned width, unsigned height)
{
    if (!width) width = this->width;
    if (!height) height = this->height;
    float* zeroes = new float[width * height * FLOATS_PER_TEXEL];
    memset(zeroes, 0, sizeof(float) * width * height * FLOATS_PER_TEXEL);
    loadDataTexture(idx, zeroes, x, y, width, height);
    delete[] zeroes;
}

////////////////////////////////////////////////////////////////
void GPOP::setInteriorColors(ColorPool pool){
    intColors = pool;
}

////////////////////////////////////////////////////////////////
void GPOP::setExteriorColors(ColorPool pool){
    extColors = pool;
}

////////////////////////////////////////////////////////////////
int GPOP::getNumInteriorColors(){
    return intColors.numColors();
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
int GPOP::getNumExteriorColors(){
    return extColors.numColors();
}

////////////////////////////////////////////////////////////////
void GPOP::setIntColorIndex(int idx){
    intColorIdx = idx;
}

////////////////////////////////////////////////////////////////
void GPOP::setExtColorIndex(int idx){
    extColorIdx = idx;
}

////////////////////////////////////////////////////////////////
void GPOP::randomizeColor(){
    intColorIdx = ofRandom(intColors.numColors());
    extColorIdx = ofRandom(extColors.numColors());
}

////////////////////////////////////////////////////////////////
ofFbo::Settings GPOP::getFboSettings(unsigned numColorBuffers) const
{
    if (!width || !height) {
        ofLogFatalError() << "GpuParticles::getFboSettings() called before GpuParticles::setup()";
        return ofFbo::Settings();
    }
    
    ofFbo::Settings s;
    s.internalformat = GL_RGBA32F_ARB;
    s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    s.minFilter = GL_NEAREST;
    s.maxFilter = GL_NEAREST;
    s.wrapModeHorizontal = GL_CLAMP;
    s.wrapModeVertical = GL_CLAMP;
    s.width = width;
    s.height = height;
    s.numColorbuffers = numColorBuffers;
    return s;
}

////////////////////////////////////////////////////////////////
ofTextureData GPOP::getTextureSettings() const
{
    if (!width || !height) {
        ofLogFatalError() << "GpuParticles::getFboSettings() called before GpuParticles::setup()";
        return ofTextureData();
    }
    
    ofTextureData d;
    d.glInternalFormat = GL_RGBA32F_ARB;
    d.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    d.minFilter = GL_NEAREST;
    d.magFilter = GL_NEAREST;
    d.wrapModeHorizontal = GL_CLAMP;
    d.wrapModeVertical = GL_CLAMP;
    d.width = width;
    d.height = height;
    return d;
}

////////////////////////////////////////////////////////////////
void GPOP::loadShaders(const string& updateShaderName, const string& drawShaderName)
{
    updateShader.load(updateShaderName);
    drawShader.load(drawShaderName);
}

////////////////////////////////////////////////////////////////
void GPOP::setRadius(float& value){
    radius = value;
}

////////////////////////////////////////////////////////////////
float GPOP::getRadius(){
    return radius;
}

////////////////////////////////////////////////////////////////
void GPOP::setNoiseScaleMin(float& value){
    noiseScaleMin = value;
}

////////////////////////////////////////////////////////////////
float GPOP::getNoiseScaleMin(){
    return noiseScaleMin;
}

////////////////////////////////////////////////////////////////
void GPOP::setNoiseScaleMax(float& value){
    noiseScaleMax = value;
}

////////////////////////////////////////////////////////////////
float GPOP::getNoiseScaleMax(){
    return noiseScaleMax;
}

////////////////////////////////////////////////////////////////
void GPOP::setNoiseFrequencyMin(float value){
    noiseFrequencyMin = value;
}

////////////////////////////////////////////////////////////////
float GPOP::getNoiseFrequencyMin(){
    return noiseFrequencyMin;
}

////////////////////////////////////////////////////////////////
void GPOP::setNoiseFrequencyMax(float value){
    noiseFrequencyMax = value;
}

////////////////////////////////////////////////////////////////
float GPOP::getNoiseFrequencyMax(){
    return noiseFrequencyMax;
}

////////////////////////////////////////////////////////////////
void GPOP::setNoiseOffset(glm::vec3& offset){
    this->offset = offset;
}

glm::vec3 GPOP::getNoiseOffset(){
    return offset;
}
