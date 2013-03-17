//
//  main.cpp
//  FP_ADL
//
//  Created by Yahoo on 11/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
/*
     Input arguments checking
     -i path/to/video: video path
     -crf : [crf]run CRF++ for activity recognition
     -show : [show]show detection result 
     -pause : [pause]show detection and pause when detected
     -start start_frame : start frame
     -end end_frame : end frame
     */
#include "main.h"

int main (int argc, const char * argv[])
{   
    cout << "FP_ADL_Detector\n";    
    
    string input_video = "";
    bool do_activity_detection = false;
    bool show_detection_result = false;
    bool pause_when_detected = false;
    int start_frame = -1;
    int end_frame = -1;
    int indicate = -1;

    string temp = argv[1];

    for(int i = 1 ; i <= argc - 1; i ++){
        string tmp;
        tmp.assign(argv[i]);

        if(tmp.compare("-crf") == 0){
            do_activity_detection = true;
        }

        if(tmp.compare("-show") == 0){
            show_detection_result = true;
        }         

        if(tmp.compare("-pause") == 0){
            pause_when_detected = true;
        }

        if(tmp.compare("-start") == 0){
            start_frame = atoi(argv[i+1]);
            i++;
        }

        if(tmp.compare("-end") == 0){
            end_frame = atoi(argv[i+1]);
            i++;
        }

        if(tmp.compare("-i") == 0){
            input_video = string(argv[i+1]);
            i++;
        }

        if(tmp.compare("-indicate") == 0){
            indicate = atoi(argv[i+1]);
            i++;
        }    
    }
    
    cout  << "input video: " << input_video << endl;
    cout << "start/end frame: " << start_frame << "/" <<end_frame <<endl;
    cout << "run crf: " << do_activity_detection << endl;
    cout << "show frames: " << show_detection_result << endl;
    cout << "pause when object detected: " << pause_when_detected << endl;
    cout << "indicate object index:" << indicate <<endl;

    if (input_video.compare("") == 0){
        cout << "Invalid input video path !\n" << endl;
        return 0;
    }

    //
    //Components initializtion
    //
    FrameModel* myFrames = new FrameModel();

    //
    //Loading input video(feature detection included)
    //
    myFrames->loadVideo_realtime(input_video, pause_when_detected, show_detection_result, start_frame , end_frame, indicate);
    cout << "Frames : " << myFrames->frame_count << endl;

    /*
    //
    //Show detection result
    //
    if(show_detection_result)
        myFrames->playVideo_with_detected_results(pause_when_detected);
    
    //
    //Building temporal pyramid
    //
    cout << "Building temporal pyramid\n";
    myTemporalPyramid->loadFrames(myFrames);
    myTemporalPyramid->showPyramid(0);
    myTemporalPyramid->buildPyramid(2);
    cout << "number of levels : " << myTemporalPyramid->num_of_levels << endl;
    myTemporalPyramid->showPyramid(1);
    
    
    //
    //Activity Detection
    //
    if(do_activity_detection)
        myTemporalPyramid->activity_detect(myFrames);
    
    
    
    
    delete myTemporalPyramid;
    */
    delete myFrames;
    
    return 0;
}


bool temp(void){
    /*
     myFrames.feature_name.push_back("fake_object");
     Rect fake;
     fake.x = 280;
     fake.y = 880;
     fake.width = 120;
     fake.height = 120;
     myFrames.frameList[3].result_list[0].push_back(fake);


     typedef vector< string > split_vector_type;
    
    split_vector_type SplitVec;
    split( SplitVec, "1 2 3", is_any_of(" ") );

    for(int i = 0 ; i < SplitVec.size() ; i ++){
        cout << SplitVec[i] << endl;
        if(SplitVec[i].compare("1") == 0)
            cout << "ok\n";
    }
     */
}

