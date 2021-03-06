

#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
    //ofSetVerticalSync(true);
	ofBackground(100);
    ofEnableAlphaBlending();
    
    gui.setup("panel");
    gui.add(backgroundThresh.setup("bgThresh", 100, 0, 255));
    gui.add(minArea.setup("minArea", 10,1, 100));
    gui.add(maxArea.setup("maxArea", 100, 101, 200));
    gui.add(contourThresh.setup("cThresh", 200, 20, 300));
    gui.add(blurAmount.setup("blur", 10, 1, 100));
    gui.add(erodeAmount.setup("erode", 0, 0, 10));
    gui.add(dilateAmount.setup("dilate", 0, 0, 10));
    gui.add(learnTime.setup("learTime", 2000, 25, 3000));
    gui.add(reset.setup("reset background"));
    gui.add(showLabels.setup("show Labels", true));
    
    gui.setPosition(550, 580);
  
    //cam.initGrabber(320, 240);
    //thresholded.allocate(320, 240, OF_IMAGE_COLOR);
    //tempImage.allocate(320, 240, OF_IMAGE_COLOR_ALPHA);
    
    //beach.loadImage("beach.jpg");
    
    
    movie.loadMovie("shopMall.mp4");
    movie.play();
    thresholded.allocate(movie.getWidth(), movie.getHeight(), OF_IMAGE_COLOR);

    
	contourFinder.setMinAreaRadius(1);
    contourFinder.setMaxAreaRadius(100);
	contourFinder.setThreshold(200);
    contourFinder.setAutoThreshold(false);
	 //wait for half a frame before forgetting something
	contourFinder.getTracker().setPersistence(50);
	// an object can move up to 32 pixels per frame
	contourFinder.getTracker().setMaximumDistance(32);
    background.setThresholdValue(200);
    
}



//------------------------------------------------------------------------------
void testApp::update() {
	
    
    //update settings from GUI
    contourFinder.setMinAreaRadius(minArea);
    contourFinder.setMaxAreaRadius(maxArea);
	contourFinder.setThreshold(contourThresh);
    background.setThresholdValue(backgroundThresh);
    background.setLearningTime(learnTime);
    int erodeNum = erodeAmount;
    int dilateNum = dilateAmount;
    
    if(reset)
    {
        background.reset();
    }
 
    movie.update();

   // cam.update();
    //tempPix.setFromPixels(cam.getPixels(), cam.getWidth(), cam.getHeight(), 3);
    //tempPix.setImageType(OF_IMAGE_COLOR_ALPHA);
    if(movie.isFrameNew())
    {
        
        
        background.update(movie, thresholded);
        thresholded.update();
        
        blur(thresholded, blurAmount);
        
        erode(thresholded, erodeNum);
        dilate(thresholded, dilateNum);
        
        contourFinder.findContours(thresholded);
        ofLog() << "number contours: " << contourFinder.size();
        
        
    }
    /*
    for(int x = 0; x < tempPix.getWidth(); x++)
    {
        for(int y = 0; y < tempPix.getHeight(); y++)
        {
            tempPix.getColor(x,y).set(255, 0);
            if(!tempPoly.inside(x,y)){
             tempImage.getColor(x, y).set(0,0,0,0);
             }
        }
    }
    
    for(int i = 0; i < contourFinder.size(); i++)
    {
        //create a polyline from the contourfinder
        ofPolyline tempPoly = contourFinder.getPolyline(i);
        
        //run through pixels
                //tempImage.update();
    }
    
  */
}


//------------------------------------------------------------------------------
void testApp::draw() {
    
    gui.draw();
	ofSetBackgroundAuto(showLabels);
	RectTracker& tracker = contourFinder.getTracker();
	ofSetColor(255);
    ofEnableAlphaBlending();
    
    ofScale(0.8, 0.8);
   // ofScale(2,2);
    
	if(showLabels) {
       movie.draw(0, 10, movie.getWidth(), movie.getHeight());
       thresholded.draw(movie.getWidth()+10, 10);  //draw the binary mask next to it
        
        //thresholded.draw(cam.getWidth() + 10, 10);
        //tempImage.setFromPixels(tempPix, tempPix.getWidth(), tempPix.getHeight(), OF_IMAGE_COLOR_ALPHA);
        //beach.draw(0,10, tempImage.getWidth(), tempImage.getHeight());
        //tempImage.draw(0,10);
        
		
        
        ofSetColor(0, 244, 0);
        ofPushMatrix();
            ofTranslate(0,10);
            contourFinder.draw();
            for(int i = 0; i < contourFinder.size(); i++) {
                ofPoint center = toOf(contourFinder.getCenter(i));
                ofPushMatrix();
                    ofTranslate(center.x, center.y);
                    int label = contourFinder.getLabel(i);
                    string msg = ofToString(i);
                    ofDrawBitmapString(msg, 0, 0);
                    ofVec2f velocity = toOf(contourFinder.getVelocity(i));
                    //ofScale(5, 5);
                    //ofLine(0, 0, velocity.x, velocity.y);
                ofPopMatrix();
            
		}
        ofPopMatrix();
         
         
	}
    else {
        movie.draw(0, 10, movie.getWidth(), movie.getHeight());
        thresholded.draw(movie.getWidth()+10, 10);  //draw the binary mask next to it

		for(int i = 0; i < contourFinder.size(); i++) {
			unsigned int label = contourFinder.getLabel(i);
			// only draw a line if this is not a new label
			if(tracker.existsPrevious(label)) {
				// use the label to pick a random color
				ofSeedRandom(label << 24);
				ofSetColor(ofColor::fromHsb(ofRandom(255), 255, 255));
				// get the tracked object (cv::Rect) at current and previous position
				const cv::Rect& previous = tracker.getPrevious(label);
				const cv::Rect& current = tracker.getCurrent(label);
				// get the centers of the rectangles
				ofVec2f previousPosition(previous.x + previous.width / 2, previous.y + previous.height / 2);
				ofVec2f currentPosition(current.x + current.width / 2, current.y + current.height / 2);
				ofLine(previousPosition, currentPosition);
			}
		}
	}
	
	// this chunk of code visualizes the creation and destruction of labels
	const vector<unsigned int>& currentLabels = tracker.getCurrentLabels();
	const vector<unsigned int>& previousLabels = tracker.getPreviousLabels();
	const vector<unsigned int>& newLabels = tracker.getNewLabels();
	const vector<unsigned int>& deadLabels = tracker.getDeadLabels();
	ofSetColor(cyanPrint);
	for(int i = 0; i < currentLabels.size(); i++) {
		int j = currentLabels[i];
		ofLine(j, 0, j, 4);
	}
	ofSetColor(magentaPrint);
	for(int i = 0; i < previousLabels.size(); i++) {
		int j = previousLabels[i];
		ofLine(j, 4, j, 8);
	}
	ofSetColor(yellowPrint);
	for(int i = 0; i < newLabels.size(); i++) {
		int j = newLabels[i];
		ofLine(j, 8, j, 12);
	}
	ofSetColor(ofColor::white);
	for(int i = 0; i < deadLabels.size(); i++) {
		int j = deadLabels[i];
		ofLine(j, 12, j, 16);
	}
    
    ofScale(1/0.8, 1/0.8);
    
    //ofScale(0.5, 0.5);
    
    // finally, a report:
	ofSetColor(0,255,0);
	stringstream reportStr;
        reportStr
        << "num blobs found " << contourFinder.size() << ", fps: " << ofGetFrameRate() << endl;
        ofDrawBitmapString(reportStr.str(), 20, 680);
    
    ofDrawBitmapString("Contour Tracking", 0, movie.getHeight()*0.8 + 20);
    ofDrawBitmapString("Background Subtraction", movie.getWidth()*0.8 + 10, movie.getHeight()*0.8 + 20);
    
    
}


//------------------------------------------------------------------------------
void testApp::keyPressed(int key) {
	
}


//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

