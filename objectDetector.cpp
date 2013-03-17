//
//  objectDetector.cpp
//  FP_ADL_Detector
//
//  Created by Yahoo on 11/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "objectDetector.h"

ObjectDetector::ObjectDetector(){

    DIR *dir;
    struct dirent *ent;
    string file_name;
    
    int counter = 0;
    int limit = -1;
    int indicate = 2;

    if ((dir = opendir (HAAR_PATH)) != NULL) {
        //print all the files and directories within directory
        while ((ent = readdir (dir)) != NULL) {
           
            file_name.assign(ent->d_name);
    
            if(file_name.compare(".") == 0 || file_name.compare("..") == 0){
                continue; 
            }
            
            //Limit the number of detectors
            if(limit!= -1 && counter == limit)break;               
            
            //Indicate the only one I want to evaluate
            if(indicate!=-1 && counter == indicate){
                //cout << "loading classifier:" << file_name << endl;

                string path = HAAR_PATH;
                path.append(file_name);
                
                Haar_cascade tmp_classifier = Haar_cascade(path , file_name);
                myHaars.push_back(tmp_classifier);
            }

            counter++;
        }

        closedir (dir);
    }else{
        //could not open directory
        cout << "No such directory:"  <<  HAAR_PATH << endl;
    }

    num_of_detectors  = myHaars.size();
    cout << "number of objectDetector: " << num_of_detectors << endl;
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