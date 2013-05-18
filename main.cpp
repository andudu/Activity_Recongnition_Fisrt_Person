//
//  main.cpp
//  FP_ADL
//
//  Created by Yahoo on 11/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "main.h"

int main (int argc, const char * argv[])
{   
    cout << "FP_ADL_Detector\n";

    //hash map for input arguments
    map<string,string> load_video_args;  
    
    load_video_args["do_activity_detection"] = "false";
    load_video_args["show_obj_detection"] = "false";
    load_video_args["show_activity_prediction"] = "false";
    load_video_args["show_pyramid"] = "false";
    load_video_args["pause_when_detected"] = "false";
    load_video_args["ground_truth_detect"] = "false";
    load_video_args["build_pyramid"] = "false";
    load_video_args["start_frame"] = "-1";
    load_video_args["end_frame"] = "-1";
    load_video_args["indicate"] = "-1";
    load_video_args["length"] = "60";
    load_video_args["thres_factor"] = "2";
    load_video_args["FPN"] = "30";

    if(argc == 1){
        cout << "need arguments!\n";
        return 0;
    }

    string temp = argv[1];

    for(int i = 1 ; i <= argc - 1; i ++){
        string tmp;
        tmp.assign(argv[i]);

        if(tmp.compare("-crf") == 0){
            load_video_args["do_activity_detection"] = "true";
        }

        if(tmp.compare("-show") == 0){
            load_video_args["show_obj_detection"] = "true";
        }         

        if(tmp.compare("-pause") == 0){
            load_video_args["pause_when_detected"] = "true";
        }

        if(tmp.compare("-ground_truth") == 0){
            load_video_args["ground_truth_detect"] = "true";
        }

        if(tmp.compare("-show_pyramid") == 0){
            load_video_args["show_pyramid"] = "true";
        }

        if(tmp.compare("-build_pyramid") == 0){
            load_video_args["build_pyramid"] = "true";
        }

        if(tmp.compare("-activity_prediction") == 0){
            load_video_args["show_activity_prediction"] = "true";
        }    

        if(tmp.compare("-start") == 0){
            load_video_args["start_frame"] = argv[i+1];
            i++;
        }

        if(tmp.compare("-length") == 0){
            load_video_args["length"] = argv[i+1];
            i++;
        }

        if(tmp.compare("-i") == 0){
            load_video_args["input_video"] = argv[i+1];
            i++;
        }

        if(tmp.compare("-crf_model_path") == 0){
            load_video_args["crf_model_path"] = argv[i+1];
            i++;
        }

        if(tmp.compare("-an") == 0){
            load_video_args["annotation_file"] = argv[i+1];
            i++;
        }

        if(tmp.compare("-indicate") == 0){
            load_video_args["indicate"] = argv[i+1];
            i++;
        }

        if(tmp.compare("-thres_factor") == 0){
            load_video_args["thres_factor"] = argv[i+1];
            i++;
        }

        if(tmp.compare("-FPN") == 0){
            load_video_args["FPN"] = argv[i+1];
            i++;
        }

        if(tmp.compare("-dpm_thres") == 0){
            load_video_args["dpm_thres"] = argv[i+1];
            i++;
        }
    }

    /*
    cout << "input video: " << input_video << endl;
    cout << "start/end frame: " << start_frame << "/" <<end_frame <<endl;
    cout << "FPN: " << FPN <<endl;
    cout << "run crf: " << do_activity_detection << endl;
    cout << "show_obj_detection: " << show_obj_detection << endl;
    cout << "pause when object detected: " << pause_when_detected << endl;
    cout << "indicate object index: " << indicate <<endl;
    cout << "ground truth detect: " << ground_truth_detect <<endl;
    cout << "thres_factor: " << thres_factor <<endl;
    cout << "build pyramid: " << build_pyramid <<endl;
    */

    if (load_video_args["input_video"].compare("") == 0){
        cout << "Invalid input video path !\n" << endl;
        return 0;
    }

    
    //
    //Components initializtion
    //
    FrameModel* myFrames;
    load_video_args["ground_truth_detect"].compare("true") == 0 ?
        myFrames = new FrameModel(true, atoi(load_video_args["FPN"].c_str()))  :
        myFrames = new FrameModel(false, atoi(load_video_args["FPN"].c_str())) ;
    
    //
    //Processing
    //
    myFrames->loadVideo_realtime(load_video_args);


    delete myFrames;
    return 0;
}


bool temp(void){
    //For testing
}

