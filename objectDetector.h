//
//  objectDetector.h
//  FP_ADL_Detector
//
//  Created by Yahoo on 11/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef FP_ADL_Detector_objectDetector_h
#define FP_ADL_Detector_objectDetector_h

#include <stdlib.h>
#include <iostream>
#include <vector>
#include "haar_cascade.h"
#include "frames.h"

#define MIN_NUM_NODES 3 //minimum number of nodes to make a possible "activity" sequence
#define MAX_DETECTION_BOX 1000
#define MIN_DETECTION_BOX 200
//#define HAAR_PATH "../../DerivedData/FP_ADL_Detector/Build/Products/Debug/"
#define HAAR_PATH ""

using namespace std;

class ObjectDetector{
	
    
public:
	
    //variable
    string name;
    string name_of_frames;
    int num_of_detectors;
    
    //public functions
    bool detect(FrameModel* frame_model , int frame_index ,IplImage* image);
    
    //constructor
	ObjectDetector();
	~ObjectDetector();
    
private:
    
    vector<Haar_cascade> myHaars;
};



#endif
