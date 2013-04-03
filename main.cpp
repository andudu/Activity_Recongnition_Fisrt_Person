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
    string annotation_file = "";
    bool do_activity_detection = false;
    bool show_detection_result = false;
    bool pause_when_detected = false;
    bool ground_truth_detect = false;
    int start_frame = -1;
    int end_frame = -1;
    int indicate = -1;
    int length = 60;
    int thres_factor = 2;

    if(argc == 1){
        cout << "need arguments!\n";
        return 0;
    }

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

        if(tmp.compare("-length") == 0){
            length = atoi(argv[i+1]);
            i++;
        }

        if(tmp.compare("-i") == 0){
            input_video = string(argv[i+1]);
            i++;
        }

        if(tmp.compare("-an") == 0){
            annotation_file = string(argv[i+1]);
            i++;
        }

        if(tmp.compare("-indicate") == 0){
            indicate = atoi(argv[i+1]);
            i++;
        }

        if(tmp.compare("-ground_truth") == 0){
            ground_truth_detect = true;
        }

        if(tmp.compare("-thres_factor") == 0){
            thres_factor = atoi(argv[i+1]);
            i++;
        }    
    }
    
    end_frame = start_frame + length;

    cout  << "input video: " << input_video << endl;
    cout << "start/end frame: " << start_frame << "/" <<end_frame <<endl;
    cout << "run crf: " << do_activity_detection << endl;
    cout << "show frames: " << show_detection_result << endl;
    cout << "pause when object detected: " << pause_when_detected << endl;
    cout << "indicate object index: " << indicate <<endl;
    cout << "ground truth detect: " << ground_truth_detect <<endl;
    cout << "thres_factor: " << thres_factor <<endl;

    if (input_video.compare("") == 0){
        cout << "Invalid input video path !\n" << endl;
        return 0;
    }

    //
    //Components initializtion
    //
    FrameModel* myFrames = new FrameModel(ground_truth_detect);

    //
    //Loading input video(feature detection included)
    //
    myFrames->loadVideo_realtime(input_video, pause_when_detected, show_detection_result, start_frame , end_frame, indicate, do_activity_detection, annotation_file, thres_factor);
    cout << "Frames : " << myFrames->frame_count << endl;

    
    cout << "================================" << "\nfinished!" << endl;
    cout  << "input video: " << input_video << endl;
    cout << "start/end frame: " << start_frame << "/" <<end_frame <<endl;
    cout << "run crf: " << do_activity_detection << endl;
    cout << "show frames: " << show_detection_result << endl;
    cout << "pause when object detected: " << pause_when_detected << endl;
    cout << "indicate object index: " << indicate <<endl;
    cout << "ground truth detect: " << ground_truth_detect <<endl;

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

