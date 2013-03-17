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
#include <fstream>
#include <vector>
#include <dirent.h>
#include <boost/algorithm/string.hpp>
#include "haar_cascade.h"
#include "frames.h"

#define MAX_DETECTION_BOX 1000
#define MIN_DETECTION_BOX 200
//#define HAAR_PATH "../../DerivedData/FP_ADL_Detector/Build/Products/Debug/"
#define HAAR_PATH "cascade_0305/"

using namespace std;
using namespace boost;

class mean_std{

public:
    
    string classifier_name;
    int width_mean;
    int height_mean;
    int width_std;
    int height_std; 

};


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
	ObjectDetector(int indicate);
	~ObjectDetector();
    
private:
    
    vector<Haar_cascade> myHaars;
    vector<mean_std> mean_std_list;

    bool mean_std_reader();
};



#endif
