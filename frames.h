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
#include "time.h"

#define NUM_FEATURE_TOTAL 89

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
    int FPN;
    int frame_start;
    int frame_count;
    int num_frames;
    int num_features;
    vector<string> feature_name;//This is for use in showing detection result
    
    //constructor
	FrameModel(bool ground_truth_detect, int FPN);
	~FrameModel();
    
    //public functions
    int getFPN();
    bool loadVideo_realtime(map<string, string> args);
    bool playVideo();
    bool showFeature(int index);
    bool playImage_with_detected_results(bool pause_when_detected, IplImage *tempFrame, string activity, string prob);
    bool print_info(string info_id);
    
private:
    string video_path;
    bool ground_truth_detect;
};

#endif
