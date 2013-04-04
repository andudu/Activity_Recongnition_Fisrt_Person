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

#define ACTIVITY_DETECT_THRESHOLD 0.45

using namespace boost;
using namespace std;


class ActivityDetector{
	
    
public:
	
    ////variable
    int thres_factor;

    ////constructor
	ActivityDetector(int);
	~ActivityDetector();
    
    ////public functions
    bool print_info(string info_id);
    bool activity_detect(TemporalPyramid *my_pyramid);
    vector<string> run_crf(TemporalPyramid *my_pyramid,int level, int node);


private:

    vector<string> reader(string path);
};

#endif
