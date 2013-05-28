//
//  frames.cpp
//  FP_ADL_Detector
//
//  Created by Yahoo on 11/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "frames.h"
#include "objectDetector.h"
#include "temporalPyramid.h"
#include "activityDetector.h"

float ObjectDetector_Evaluation(int frame_index,TemporalPyramid* my_pyramid,ObjectDetector* myObjDetector){

    
    for (int i = 0 ;  i < my_pyramid->pyramid[0].size(); i++) {
        for (int j = 0; j < my_pyramid->pyramid[0][i].feature.size() ; j++) {
            //cout << my_pyramid->pyramid[level_index][i].feature[j] <<" "; 
            cout << myObjDetector->myHaars[j].name << " " << my_pyramid->pyramid[0][i].feature[j] <<endl;
        }
    }

    return 0;
}


int FrameModel::getFPN(){

    return FPN;
}


string get_activity_index(string activity){

    if(activity.compare("combing_hair") == 0 ){
        return "1 0";
    }
    if(activity.compare("make_up") == 0 ){
        return "2 0";
    }
    if(activity.compare("brushing_teeth") == 0 ){
        return "3 0";
    }
    if(activity.compare("dental_floss") == 0 ){
        return "4 0";
    }
    if(activity.compare("washing_hands_face") == 0 ){
        return "5 0";
    }
    if(activity.compare("drying_hands_face") == 0 ){
        return "6 0";
    }


    if(activity.compare("laundry") == 0 ){
        return "9 0";
    }
    if(activity.compare("laundry_stage_1") == 0 ){
        return "9 1";
    }
    if(activity.compare("laundry_stage_2") == 0 ){
        return "9 2";
    }
    if(activity.compare("washing_dishes") == 0 ){
        return "10 0";
    } 
    if(activity.compare("moving_dishes") == 0 ){
        return "11 0";
    }
    if(activity.compare("making_tea") == 0 ){
        return "12 0";
    }
    if(activity.compare("making_tea_stage_1") == 0 ){
        return "12 1";
    }
    if(activity.compare("making_tea_stage_2") == 0 ){
        return "12 2";
    }
    if(activity.compare("making_coffee") == 0 ){
        return "13 0";
    }
    if(activity.compare("making_coffee_stage_1") == 0 ){
        return "13 1";
    }
    if(activity.compare("making_coffee_stage_2") == 0 ){
        return "13 2";
    }
    if(activity.compare("drinking_water_bottle") == 0 ){
        return "14 0";
    }
    if(activity.compare("drinking_water_tap") == 0 ){
        return "15 0";
    }
    if(activity.compare("making_hot_food") == 0 ){
        return "16 0";
    }
    if(activity.compare("making_hot_food_stage_1") == 0 ){
        return "16 1";
    }
    if(activity.compare("making_hot_food_stage_2") == 0 ){
        return "16 2";
    }

    if(activity.compare("making_cold_food_snack") == 0 ){
        return "17 0";
    }
    

    if(activity.compare("vacuuming") == 0 ){
        return "20 0";
    }


    if(activity.compare("watching_tv") == 0 ){
        return "22 0";
    } 
    if(activity.compare("using_computer") == 0 ){
        return "23 0";
    }
    if(activity.compare("using_cell") == 0 ){
        return "24 0";
    }

    return "-1 0";
}

bool FrameModel::loadVideo_realtime(map<string, string> args){
    
    int indicate = atoi(args["indicate"].c_str());
    int thres_factor = atoi(args["thres_factor"].c_str());
    int start= atoi(args["start_frame"].c_str());
    int end = start + atoi(args["length"].c_str());

    float dpm_thres = atof(args["dpm_thres"].c_str());

    string crf_model = args["crf_model_path"];
    string annotation_file = args["annotation_file"];
    string path = args["input_video"];

    bool build_pyramid = false;
    bool ground_truth_detect = false;
    bool show_obj_detection = false;
    bool do_activity_detection = false;
    bool show_activity_prediction = false;
    bool pause_when_detected = false;
    bool show_pyramid = false;

    if (args["ground_truth_detect"].compare("true") == 0){
        ground_truth_detect = true;
    }

    if (args["show_obj_detection"].compare("true") == 0){
        show_obj_detection = true;
    }

    if (args["build_pyramid"].compare("true") == 0){
        build_pyramid = true;
    }

    if (args["do_activity_detection"].compare("true") == 0){
        do_activity_detection = true;
    }

    if (args["show_activity_prediction"].compare("true") == 0){
        show_activity_prediction = true;
    }

    if (args["pause_when_detected"].compare("true") == 0){
        pause_when_detected = true;
    }

    if (args["show_pyramid"].compare("true") == 0){
        show_pyramid = true;
    }
    
    TemporalPyramid* myTemporalPyramid = new TemporalPyramid();
    //ObjectDetector* myObjDetector = new ObjectDetector(indicate);
    ObjectDetector* myObjDetector = new ObjectDetector();
    ActivityDetector* myActivityDetector = new ActivityDetector(thres_factor, crf_model, build_pyramid);

    Mat grab_frame;
    IplImage frame;
    frameNode temp;
    char buffer [512];
    vector<string> activity_result;
    FILE* fp; //Output file for evaluation
    fp = fopen("activity_result.txt", "w");

    cvNamedWindow("Obj Detection Result", CV_WINDOW_AUTOSIZE);
    cvMoveWindow("Obj Detection Result", 50, 0);

    //Load ground truth obj annotation
    //myObjDetector->load_ground_truth_obj_annotation(annotation_file);
    //Load dpm detection
    //myObjDetector->load_dpm_obj_detection(annotation_file, dpm_thres);

    frame_count = end - start + 1;
    frame_start = start;

    for(int i = 0 ; i < frame_count ; i ++)
    {   
        
        cout << i << "/" << frame_count - 1<< endl;            

        sprintf(buffer,"%s/%06d.jpg",path.c_str(),start+i);
        grab_frame = imread(buffer, CV_LOAD_IMAGE_COLOR);
        frame = IplImage(grab_frame);

        num_frames++;
        frameList.push_back(temp);

        if(ground_truth_detect){
            //Ground truth detect
            myObjDetector->ground_truth_detect(this, i, &frame , frame_start);
        }else{
            //Real detect
            myObjDetector->detect(this, i, &frame);
        }            
        
        if(show_obj_detection){
            playImage_with_detected_results(pause_when_detected, &frame, myTemporalPyramid->current_best_activity, myTemporalPyramid->current_best_prob);   
        }           

        if((i%FPN) == 0){

            cout << "=================================\n";

            //Loading frames and put them into pyramid, level 0
            //Return false if it is similar to the latest one
            if(myTemporalPyramid->loadFrames_realtime(this, i)){

                //Obj detection evaluation
                ObjectDetector_Evaluation(i,myTemporalPyramid,myObjDetector);

                //Build the pyramid
                if(build_pyramid){
                    myTemporalPyramid->buildPyramid_realtime();    
                }                
                
                //Refresh the Pyramid
                myTemporalPyramid->refreshPyramid_realtime();

                //Activity Detection
                //cout << "num_features:" << num_features << endl;
                if(do_activity_detection && num_features == NUM_FEATURE_TOTAL && i > 0){
                    myActivityDetector->activity_detect(myTemporalPyramid);
                    //myActivityDetector->activity_detect_cvpr_12(myTemporalPyramid);
                }
                
                if(show_activity_prediction){
                    activity_result = myTemporalPyramid->showCurrentPrediction();
                    //cout <<  activity_result[0] << endl;
                    //Output the activity detected for further evaluation
                    if(activity_result.size() == 4){
                        fprintf(fp, "%d %s %s\n",i+frame_start-FPN,get_activity_index(activity_result[2]).c_str(),activity_result[3].c_str());   
                    }else if(activity_result.size() == 2){
                        fprintf(fp, "%d %s %s\n",i+frame_start-FPN,get_activity_index(activity_result[0]).c_str(),activity_result[1].c_str());
                    }else{
                        //Do nothing
                    }                
                }
                     
            }else{
                cout << "Not adding new node because its similar to the latest one!" << endl;
                cout << "Remaining latest prediction" << endl;
            }           

            //Display
            if(show_pyramid){
                myTemporalPyramid->print_info("pyramid");
            }
        }
        

        if(cvWaitKey(10) >= 0)
            break;
    }

    cvDestroyWindow("Obj Detection Result");
    delete myObjDetector;
    delete myTemporalPyramid;
    fclose(fp);

    return true;
}

bool FrameModel::playVideo(){
    
    if (frameList.size() < 1) {
        return false;
    }
    
    cvNamedWindow("AVI player",0);
    
    ////Show frames grabed
    cout << "Playing the selected frames\n";
    for(unsigned int i = 0; i < frameList.size(); ++i){
        
        //cvShowImage("AVI player",frameList[i].frame);
        
        if(cvWaitKey(10) >= 0)
            break;
    }
    
    cvWaitKey();
    cvDestroyWindow("AVI player");
    
    return true;
}

bool FrameModel::showFeature(int index){
    
    if (frameList.size() < index + 1) {
        return false;
    }
    
    /*
     Show frames
     */
    cout << "frame:" << index << " fetures:\n";
    for(unsigned int i = 0; i < frameList[index].feature.size(); ++i){
        cout << frameList[index].feature[i]<< " ";
    }
    
    cout << endl;
    
    return true;
}

bool FrameModel::playImage_with_detected_results(bool pause_when_detected, IplImage *tempFrame, string activity, string prob){

    int detection_counter = 0;

    //cout << "num_features : " << num_features << endl;

    for(int feature_index = 0 ; feature_index < num_features ; feature_index ++)
    {   
        //cout << "feature_index : " << feature_index << " " << feature_name[feature_index] << endl;
        //cout << "num of detections : " << frameList.back().result_list[feature_index].size() << endl;
        //cout << "feature_index" << feature_index <<endl;   
        for (int box = 0 ; box < frameList.back().result_list[feature_index].size() ; box++){     
            //cout << "drawing bboxes : " << box << endl;
            CvPoint point1, point2;  
            //cout << "box:" << box <<endl; 
            point1.x = frameList.back().result_list[feature_index][box].x;  
            point2.x = frameList.back().result_list[feature_index][box].x + frameList.back().result_list[feature_index][box].width;  
            point1.y = frameList.back().result_list[feature_index][box].y;  
            point2.y = frameList.back().result_list[feature_index][box].y + frameList.back().result_list[feature_index][box].height;  
            //cout << "x1:" << point1.x << " x2:" << point2.x << " y1:"<< point1.y << " y2:" <<point2.y <<endl;
            //Scout << "width:" << point2.x - point1.x << " height:" << point2.y - point1.y << endl;
            cvRectangle(tempFrame, point1, point2, CV_RGB(0,255,0), 3, 8, 0);
            /*
             Put the object name on the box
             */
            CvFont font;
            cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);
            cvPutText(tempFrame, feature_name[feature_index].c_str(), cvPoint(point1.x+10, point1.y+20), &font, cvScalar(255, 0, 0, 0));
            detection_counter++;
        } 
    }
    CvFont font_act_label;
    cvInitFont(&font_act_label, CV_FONT_HERSHEY_SIMPLEX, 1.5, 1.5, 0, 3, CV_AA);
    cvPutText(tempFrame, activity.c_str(), cvPoint(20, 50), &font_act_label, cvScalar(0, 255, 0, 0));
    cvPutText(tempFrame, prob.c_str(), cvPoint(20 , 100), &font_act_label, cvScalar(0, 255, 0, 0));
    
    /*
     scale down the image since it's 720x1280 sometimes exceeds the monitor size
     */
    IplImage *dst = 0;       
    float scale = 0.5; 
    CvSize dst_cvsize;      
    dst_cvsize.width = tempFrame->width * scale;
    dst_cvsize.height = tempFrame->height * scale;
    dst = cvCreateImage( dst_cvsize, tempFrame->depth, tempFrame->nChannels);
    cvResize(tempFrame, dst, CV_INTER_LINEAR);
    
    /*
     Show the result in the window.  
     */
    cvShowImage("Obj Detection Result", dst);
    if(pause_when_detected && detection_counter > 0)
        cvWaitKey(0);

    cvReleaseImage(&dst);

    return true;
}

bool FrameModel::print_info(string info_id){
    
    if(info_id.compare("num_frames") == 0){
        cout << "num_frames:" << num_frames << endl;
        return true;
    }

    return false;
}

FrameModel::FrameModel(bool _ground_truth_detect, int _FPN){
    ground_truth_detect = _ground_truth_detect;
    num_frames = 0;
    FPN = _FPN;
}

FrameModel::~FrameModel(){
}
