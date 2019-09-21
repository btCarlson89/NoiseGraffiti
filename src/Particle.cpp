//
//  Particle.cpp
//  noisePaint
//
//  Created by Brian Carlson on 8/30/19.
//

#include "Particle.h"
#include "POP.h"

Particle::Particle(POP* parent){
    stepSize = ofRandom(1, 3);
    noiseZ = ofRandom(-0.01, 0.01);
    dNoiseZ = 0.00001;
    noiseScale = 200.0;
    noiseStrength = 1.0;
    width = parent->getWidth();
    height = parent->getHeight();
    radius = 1.0f;
    
    origin = glm::vec3(width / 2, height / 2, 0);
    
    pos = glm::vec3(ofRandom(width), ofRandom(height), 0);
    
    this->parent = parent;
}

void Particle::update(){
    noiseScale = ofMap(ofDist(pos.x, pos.y, noiseZ, origin.x, origin.y, 0.0), 0.4 * height, 0.5 * height, parent->getNoiseScaleMin(), parent->getNoiseScaleMax(), true);
    
    noiseStrength = ofMap(ofDist(pos.x, pos.y, noiseZ, origin.x, origin.y, 0.0), 0.4 * height, 0.5 * height, parent->getNoiseStrengthMin(), parent->getNoiseStrengthMax(), true);
    
    angle = ofNoise((pos.x + parent->getOffset().x) / noiseScale, (pos.y + parent->getOffset().y) / noiseScale, noiseZ + parent->getOffset().z) * TWO_PI * noiseStrength;
    
    pos.x += cos(angle) * stepSize;
    pos.y += sin(angle) * stepSize;
    
    noiseZ += dNoiseZ;
    
    // offscreen wrap
    if (pos.x < -10 ){
        pos.x = width + 10;
    }
    if (pos.x > width + 10){
        pos.x = -10;
    }
    if (pos.y < -10){
        pos.y = height + 10;
    }
    if (pos.y > height + 10){
        pos.y = -10;
    }
}

void Particle::draw(){
    ofDrawCircle(pos.x, pos.y, radius);
}
