#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxGui.h"


class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
    void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
    
    //ofxCV declarations
	float threshold;
	ofxCv::ContourFinder contourFinder;
	//bool showLabels;
    //ofVideoGrabber cam;
    ofVideoPlayer movie;
    
    ofxCv::RunningBackground background;
    ofImage thresholded;
    ofImage tempImage;
    ofImage beach;

    
    ofxIntSlider backgroundThresh;
    ofxIntSlider minArea;
    ofxIntSlider maxArea;
    ofxIntSlider contourThresh;
    ofxIntSlider blurAmount;
    ofxIntSlider erodeAmount;
    ofxIntSlider dilateAmount;
    ofxIntSlider learnTime;
    ofxButton reset;
    ofxToggle showLabels;
    ofxPanel gui;
    
    ofPolyline polyFromContour;
    
    ofPixels tempPix;
    
    
};
