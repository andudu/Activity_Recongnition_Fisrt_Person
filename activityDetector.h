//
//  activityDetection.h
//  FP_ADL_Detector
//
//  Created by Yahoo on 11/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef FP_ADL_Detector_activityDetector_h
#define FP_ADL_Detector_activityDetector_h

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include "temporalPyramid.h"

#define ACTIVITY_DETECT_THRESHOLD 0.5

using namespace boost;
using namespace std;

class ActivityDetector{
	
    
public:
	
    ////variable
    float thres_factor;
    string crf_model;
    bool build_pyramid;

    ////constructor
	ActivityDetector(int thres, string crf_model, bool build_pyramid);
	~ActivityDetector();
    
    ////public functions
    bool print_info(string info_id);
    bool activity_detect(TemporalPyramid *my_pyramid);
    bool activity_detect_cvpr_12(TemporalPyramid *my_pyramid);
    vector<string> run_crf(TemporalPyramid *my_pyramid,int level, int node);
    vector<string> run_crf(TemporalPyramid *my_pyramid, int level_1, int node_1, int level_2, int node_2);


private:

    vector<string> reader(string path);
};

#endif
