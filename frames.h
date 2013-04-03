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
#include <math.h>
#include <boost/algorithm/string.hpp>

//#define MIN_NUM_ACT_SEQUENCE 3 //minimum number of nodes to make a possible "activity" sequence

using namespace std;
using namespace cv;
using namespace boost;

class frameNode{

public:
    
    vector<float> feature;
    vector< vector<Rect> > result_list;

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
    vector<string> feature_name;//This is for use in showing detection result
    
    //constructor
	FrameModel(bool);
	~FrameModel();
    
    //public functions
    int getFPS();
    bool loadVideo_realtime(string path, bool pause_when_detected, bool show_detection_result, int start, int end ,int indicate, bool do_activity_detection, string annotation_file,int thres_factor);
    bool playVideo();
    bool showFeature(int index);
    bool playVideo_with_detected_results(bool pause_when_detected);
    bool playImage_with_detected_results(bool pause_when_detected, IplImage *frame);
    bool print_info(string info_id);
    
private:
    string video_path;
    bool ground_truth_detect;
};

#endif
