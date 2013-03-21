//
//  frames.h
//  FP_ADL_Detector
//
//  Created by Yahoo on 11/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef FP_ADL_Detector_frames_h
#define FP_ADL_Detector_frames_h

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include <fstream>
#include <map>
#include <boost/algorithm/string.hpp>

#define MIN_NUM_ACT_SEQUENCE 3 //minimum number of nodes to make a possible "activity" sequence

using namespace std;
using namespace cv;
using namespace boost;

class frameNode{

public:
    
    //IplImage* frame;
    vector<float> feature;
    vector< vector<Rect> > result_list;
    string activity;

};

class obj_info{

public:
    bool exist;
    int frame,x,y,width,height,index;
    string obj_name;

    obj_info(){
        exist = false;
    }
};


class frame_annotation{

public:
    map<int,obj_info> objs;
};


class FrameModel{
	
    
public:
	
    //variable
    string name;
    vector<frameNode> frameList;
    int FPS;
    int frame_start;
    int frame_count;
    int num_frames;
    int num_features;
    vector<string> feature_name;
    //map<frame,annotation>
    map<int, frame_annotation> ground_truth;

    
    //constructor
	FrameModel();
	~FrameModel();
    
    //public functions
    int getFPS();
    bool load_ground_truth_obj_annotation(string path);
    bool loadVideo_realtime(string path, bool pause_when_detected, bool show_detection_result, int start, int end ,int indicate);
    bool playVideo();
    bool showFeature(int index);
    bool playVideo_with_detected_results(bool pause_when_detected);
    bool playImage_with_detected_results(bool pause_when_detected, IplImage *frame);
    
private:
    string video_path;
    bool ground_truth_obj_annotation_reader(int video_index);    
};

#endif
