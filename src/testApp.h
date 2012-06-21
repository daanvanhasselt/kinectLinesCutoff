#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    ofVideoPlayer videoPlayer;

    int numCols;
    int numRows;
    vector<ofVec3f *> points;
    
    ofShader shader;    // declare the shader
};
