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

float ObjectDetector_Evaluation(int frame_index,TemporalPyramid* my_pyramid){

    int level_index = 0;

    cout << "Number of nodes in level " << level_index  << " => " << my_pyramid->pyramid[level_index].size() <<endl;;
    cout << "Node features" << endl;
    for (int i = 0 ;  i < my_pyramid->pyramid[level_index].size(); i++) {
        for (int j = 0; j < my_pyramid->pyramid[level_index][i].feature.size() ; j++) {
            cout << my_pyramid->pyramid[level_index][i].feature[j] <<" "; 
        }
        cout << " | ";
    }
    cout << "\n";

    return 0;
}


int FrameModel::getFPS(){

    return FPS;
}

bool FrameModel::loadVideo_realtime(string path, bool pause_when_detected ,bool show_detection_result,int start, int end, int indicate, bool do_activity_detection, string annotation_file){
    
    ObjectDetector* myObjDetector = new ObjectDetector(indicate);
    TemporalPyramid* myTemporalPyramid = new TemporalPyramid();
    ActivityDetector* myActivityDetector = new ActivityDetector();
    VideoCapture capture(path.c_str());
    Mat grab_frame;
    IplImage frame;
    frameNode temp;
    char buffer [512];

    cvNamedWindow("Obj Detection Result", CV_WINDOW_AUTOSIZE);
    cvMoveWindow("Obj Detection Result", 50, 0);
    
    //Load ground truth obj annotation
    myObjDetector->load_ground_truth_obj_annotation(annotation_file);

    FPS = 29;
    cout << "FPS :"<< FPS <<endl;
    
    frame_count = end - start + 1;
    frame_start = start;

    for(int i = 0 ; i < frame_count ; i ++)
    {   
        //cout << i + 1<< "/" << frame_count << endl;            
        sprintf(buffer,"%s/%06d.jpg",path.c_str(),start+i);
        //cout << buffer <<endl;
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
        
        if(show_detection_result){
            playImage_with_detected_results(pause_when_detected, &frame);   
        }           
        
        //Loading frames and put them into pyramid, level 0
        myTemporalPyramid->loadFrames_realtime(this);

        if((i%FPS) == 0){
            //Build the pyramid
            myTemporalPyramid->buildPyramid(num_frames, FPS);

            //Showing pyramids
            myTemporalPyramid->print_info("pyramid");
        }
        
        //Activity Detection
        if(do_activity_detection && (i%FPS) == 0){

            //myTemporalPyramid->activity_detect(this, MIN_NUM_ACT_SEQUENCE);
            myActivityDetector->activity_detect(myTemporalPyramid);

        }
        
        /*
        if((i%FPS) == 0){
            ObjectDetector_Evaluation(i,myTemporalPyramid);  
        }
        */

        if(cvWaitKey(10) >= 0)
            break;
    }
    
    //cvReleaseCapture(&capture);
    capture.release();    
    cvDestroyWindow("Obj Detection Result");
    delete myObjDetector;
    delete myTemporalPyramid;
    
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

bool FrameModel::playImage_with_detected_results(bool pause_when_detected, IplImage *tempFrame){

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

FrameModel::FrameModel(bool _ground_truth_detect){
    ground_truth_detect = _ground_truth_detect;
    num_frames = 0;
}

FrameModel::~FrameModel(){
}
