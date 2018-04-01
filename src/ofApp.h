// ofxFaceTrack2 example: SIMPLE EXAMPLE
//
// This example shows minimal usage of the ofxFaceTracker2 addon,
// doing simple landmark detection and pose estimation
//

#pragma once

#include "ofMain.h"
#include "ofxFaceTracker2.h"
#include "ofxImgWarp.h"
#include "ofxXmlSettings.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    //void keyPressed(int key);
    //void keyReleased(int key);
    //void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    //void mouseReleased(int x, int y, int button);
    //void windowResized(int w, int h);
    //void dragEvent(ofDragInfo dragInfo);
    //void gotMessage(ofMessage msg);
    
    ofImage image;
    ofPixels dst;
    ofTexture tex;
    
    vector<pair<ofPoint,ofPoint> > ctrl;
    
    ofVec2f leftEye;
    ofVec2f rightEye;
    
    ofxXmlSettings settings;
    
    ofxImgWarp imgWarp;
    
    int pointNum = 0;
    int arr[30][2];
    int countFrame = 0;
    
    ofxFaceTracker2 tracker;
    //ofxFaceTracker2 tracker2;
    ofVideoGrabber grabber;
    
    vector<ofPoint> cameraFace;
};
