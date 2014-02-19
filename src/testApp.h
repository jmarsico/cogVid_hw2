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
	//ofVideoPlayer movie;
	ofxCv::ContourFinder contourFinder;
	bool showLabels;
    ofVideoGrabber cam;
    
    ofxCv::RunningBackground background;
    ofImage thresholded;
    ofImage edge;

    ofImage grabFrame;
    //int backgroundThresh;
    
    ofxIntSlider backgroundThresh;
    ofxIntSlider minArea;
    ofxIntSlider maxArea;
    ofxIntSlider contourThresh;
    ofxIntSlider blurAmount;
    ofxIntSlider erodeAmount;
    ofxIntSlider learnTime;
    ofxButton reset;
    ofxPanel gui;
    
};
