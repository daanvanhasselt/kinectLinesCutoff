#include "testApp.h"

// lets define drawing_mode: 0 = draw colored boxes, 1 is draw horizontal lines
#define DRAWING_MODE_BOXES 0	// good practice!
#define DRAWING_MODE_LINES 1
#define DRAWING_MODE DRAWING_MODE_LINES

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    
    videoPlayer.loadMovie("kinect_test.mov");
    videoPlayer.play();
    videoPlayer.setPixelFormat(OF_PIXELS_RGB);
    
    numCols = 50;
    numRows = 50;
    
    // we're going to have to fill the vector with ofvec3fs; //we're allocating memory for each vertor since we're using points in update()
    for(int i = 0; i < numCols * numRows; i++){    // numCols * numRows = total amount of points
        points.push_back(new ofVec3f(0,0,0));    // push back a new ofVec3f
    }
    
    glEnable(GL_DEPTH_TEST);
    ofEnableSmoothing();
    
    shader.load("shaders/shader");
}

//--------------------------------------------------------------
void testApp::update(){
    videoPlayer.update();    

    if(videoPlayer.isFrameNew()){        
        
        unsigned char *pixels = videoPlayer.getPixels();
        
        // loop though all the pixels
        for(int y = 0; y < numRows; y++){
            for(int x = 0; x < numCols; x++){
                // calculate the coordinates in the image instead of in the grid
                // (0, 0) -> (0, 0) 
                // (9, 9) -> (640, 480)
                
                int kinectX = ofMap(x, 0, numCols, 0, videoPlayer.getWidth());
                int kinectY = ofMap(y, 0, numRows, 0, videoPlayer.getHeight());
                int kinectIndex = kinectY * videoPlayer.getWidth() + kinectX;
                kinectIndex *= 3;                    // index in kinect frame (RGB)
                
                int index = y * numCols + x;    // index in grid
                int depthValue = pixels[kinectIndex];    // get the depth value from the kinect frame
                ofVec3f *point = points[index];
                point->set(kinectX, kinectY, depthValue);
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    static ofEasyCam cam;
    cam.begin();
    ofTranslate(-videoPlayer.getWidth() / 2.0, videoPlayer.getHeight() / 2.0);
    ofScale(1, -1);
    
	if(DRAWING_MODE == DRAWING_MODE_BOXES){
		// draw boxes
        for(int i = 0; i < numCols * numRows; i ++){
            ofVec3f *point = points[i];
            ofVec3f zeta = *point;  
            
            
            int hue = ofMap(zeta.z, 0, 255, 200, 255); 
            int sizez= ofMap(zeta.z, 0, 255, 9, 25); 
            
            ofColor c;
            hue += sin(ofGetElapsedTimeMillis() / 1000.0) * 100.0;
            if(hue > 255){
                hue -= 255;
            }
            c.setHsb(hue,  255, 255);
            ofFill();
            
            ofSetColor(c);
            ofBox(*point, sizez);    // position, size
        }

    }
    
    if(DRAWING_MODE == DRAWING_MODE_LINES){
        // draw lines
        for(int y = 0; y < numRows; y++){
            ofSetLineWidth(2);
            ofNoFill();
            ofBeginShape();
            for(int x = 0; x < numCols; x++){
                int index = y * numCols + x;
                ofPoint prevPoint = *points[ ofClamp(index - 1, 0, numCols * numRows - 1) ];
                ofPoint curPoint = *points[ ofClamp(index, 0, numCols * numRows - 1) ];
                if(fabs(prevPoint.z - curPoint.z) > 100){
                    ofEndShape();
                    ofBeginShape();
                }
                ofVertex(curPoint);
//                ofCurveVertex(curPoint);
            }
            ofEndShape();
        }
    }
    
    cam.end();
    
    ofDrawBitmapString(ofToString(ofGetFrameRate(), 0), 20, 20);

}