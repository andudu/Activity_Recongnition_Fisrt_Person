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
#include <string>
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
#define HAAR_PATH "cascade_0404/"

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

class obj_info{

public:
    bool exist;
    float dpm_score;
    int frame,x,y,width,height,index;
    string name;

    obj_info(){
        exist = false;
    }
};


class frame_annotation{

public:
    // <obj_index,obj_info>
    map<int,obj_info> objs;
};


class ObjectDetector{
	
    
public:
	
    //variable
    string name;
    string name_of_frames;
    int num_of_detectors;
    
    map<int, frame_annotation> ground_truth;//<frame_index,frame_annotation>
    map<int, string> obj_name;
    map<string, int> obj_name_reverse;
    
    //public functions
    bool detect(FrameModel* frame_model , int frame_index ,IplImage* image);
    bool load_ground_truth_obj_annotation(string path);
    bool load_dpm_obj_detection(string path);
    bool ground_truth_detect(FrameModel* frame_model , int frame_index ,IplImage* image, int frame_start);
    
    //constructor
	ObjectDetector(int indicate);
	~ObjectDetector();
    
private:
    
    vector< vector<Rect> > result_list_cached;
    vector<Haar_cascade> myHaars;
    vector<mean_std> mean_std_list;

    bool mean_std_reader();
    vector<string> reader(string path);
    bool ground_truth_obj_annotation_reader(int video_index);
};



#endif
