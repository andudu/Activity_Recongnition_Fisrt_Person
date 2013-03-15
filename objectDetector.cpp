//
//  objectDetector.cpp
//  FP_ADL_Detector
//
//  Created by Yahoo on 11/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "objectDetector.h"

ObjectDetector::ObjectDetector(){
    string path = HAAR_PATH;
    path.append("cascade_active_beverage.xml");
    
    Haar_cascade tmp = Haar_cascade(path , "active_beverage");
    myHaars.push_back(tmp);
    
    path = HAAR_PATH;
    path.append("cascade_active_laptop.xml");
    tmp = Haar_cascade(path , "active_laptop");
    myHaars.push_back(tmp);
    
    path = HAAR_PATH;
    path.append("cascade_active_mug.xml");
    tmp = Haar_cascade(path , "active_mug");
    myHaars.push_back(tmp);
    
    path = HAAR_PATH;
    path.append("cascade_active_water_dispenser.xml");
    tmp = Haar_cascade(path , "active_water_dispenser");
    myHaars.push_back(tmp);
}

ObjectDetector::~ObjectDetector(){
}

/*
 Single frame detection
 */
bool ObjectDetector::detect(FrameModel* frame_model , int frame_index ,IplImage* image){
    
    name_of_frames = frame_model->name;
    
    if( frame_index == 0)//If this is the first detection
        frame_model->num_features = (int)myHaars.size();//Equal to num of object detectors
    
    //Detection using the cascade classifiers in myHaars
    for (int cls = 0 ; cls < frame_model->num_features ; cls ++){
        
        if( frame_index == 0)
            frame_model->feature_name.push_back(myHaars[cls].name);//If this is the first detection
            
        //cout << "detecting:" <<<< "/" << frame_model->frameList.size()-1 << endl; 
        vector<Rect> result_list = myHaars[cls].detect(image,MIN_DETECTION_BOX,MAX_DETECTION_BOX);
        frame_model->frameList[frame_index].feature.push_back(result_list.size());
        frame_model->frameList[frame_index].result_list.push_back(result_list);
        
        
    }
    
    return true;
}