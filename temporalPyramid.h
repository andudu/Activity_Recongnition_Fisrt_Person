//
//  temporalPyramid.h
//  FP_ADL_Detector
//
//  Created by Yahoo on 11/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef FP_ADL_Detector_temporalPyramid_h
#define FP_ADL_Detector_temporalPyramid_h

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <boost/algorithm/string.hpp>
#include "frames.h"

#define CORRELATION_THRES 0.3

using namespace boost;
using namespace std;

class table_element{

public:
    
    string activity;
    
    float prob;

    class last_node{
        int level;
        int node;
    };

    table_element(){
        activity = "NULL";
        prob = -1;
    };
};

class node{
    
public:
    vector<float> feature;
    table_element table[2][2];
    bool abandoned;
    bool table_filled;

    node(){
        abandoned = false;
        table_filled = false;
    };
};

class prediction_unit{
public:
    int level;
    int node;
    int table_row;
    int table_col;
};

class TemporalPyramid{
	
    
public:
	
    ////variable
    string name;
    int frame_per_node;
    int num_of_features;
    int num_of_levels;
    vector< vector<node> > pyramid;
    vector<prediction_unit> current_prediction;//The prediction made in this time step

    ////constructor
	TemporalPyramid();
	~TemporalPyramid();
    
    ////public functions
    //load frames and build the first level pyramid(nodes)
    bool loadFrames(FrameModel* frames);
    bool loadFrames_realtime(FrameModel* frames, int frame_index);
    bool buildPyramid(int frame_size ,int fps);
    bool buildPyramid_realtime();
    bool refreshPyramid_realtime();
    bool showPyramid(int level_index);//start from level 0
    bool print_info(string info_id);

};

#endif
