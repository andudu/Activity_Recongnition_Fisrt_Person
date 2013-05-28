//
//  haar_training.h
//  FP_ADL_Detector
//
//  Created by Yahoo on 12/13/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef FP_ADL_Detector_haar_cascade_h
#define FP_ADL_Detector_haar_cascade_h

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include "time.h"

using namespace std;
using namespace cv;


class Haar_cascade{
	
    
public:
	
    //variables
    string name;
    string classifier_name;
    CvSize min_obj_size;
    CvSize max_obj_size;
    
    //constructor
    Haar_cascade(string classifier_name , string feature_name);
	Haar_cascade(string classifier_name , string feature_name,int width_mean,int height_mean, int width_std,int height_std);
	~Haar_cascade();
    
    //public functions
    string get_name();
    int detect(string img,int min_obj_height,int min_obj_width);
    vector<Rect> detect(IplImage* image_detect);
    
    
private:
    
    CascadeClassifier myClassifier;
    
};


#endif
