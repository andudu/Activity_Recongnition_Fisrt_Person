//
//  haar_training.cpp
//  FP_ADL_Detector
//
//  Created by Yahoo on 12/13/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "haar_cascade.h"

Haar_cascade::Haar_cascade(string classifier_name , string feature_name){
    
    name = feature_name;

    // Create classifier object 
    if (myClassifier.load(classifier_name))
        cout << classifier_name << " is loaded\n";
    else
        cout << "Failed loading" << classifier_name <<endl;

    cout << classifier_name <<endl;
}

Haar_cascade::Haar_cascade(string classifier_name , string feature_name,int width_mean,int height_mean, int width_std,int height_std){
    
    name = feature_name;

    // Create classifier object 
    if (myClassifier.load(classifier_name))
        cout << classifier_name << " is loaded\n";
    else
        cout << "Failed loading" << classifier_name <<endl;

    //Tunning factor
    float f = 1;
    min_obj_size.height = height_mean - f*height_std;
    max_obj_size.height = height_mean + f*height_std;
    min_obj_size.width = width_mean - f*width_std;
    max_obj_size.width = width_mean + f*width_std;

    cout << classifier_name <<endl;
    cout << "min_obj_size.height:" << min_obj_size.height <<endl;
    cout << "min_obj_size.width:" << min_obj_size.width <<endl;
    cout << "max_obj_size.height:" << max_obj_size.height <<endl;
    cout << "max_obj_size.height:" << max_obj_size.height <<endl;
}

Haar_cascade::~Haar_cascade(){
}

string Haar_cascade::get_name(){
    
    return name;
}

vector<Rect> Haar_cascade::detect(IplImage* image_detect){
    
    clock_t start_time, end_time;
    float total_time = 0;
    vector<Rect> result_list; //The list conaining detection results    
    
    //transfer image type to CV_8U
    IplImage* tempFrame = cvCreateImage(cvSize(image_detect->width,   
                                               image_detect->height), IPL_DEPTH_8U, image_detect->nChannels);
    // Copy the current frame into the temporary image.  Also, make   
    // sure the images have the same orientation.  
    if(image_detect->origin == IPL_ORIGIN_TL)  
    {  
        cvCopy(image_detect, tempFrame, 0);  
    }  
    else  
    {  
        cvFlip(image_detect, tempFrame, 0);  
    } 
    

    
    Mat img(tempFrame,0);    
    
    //Run detection
    //cout << "Run detection at min width: "<<min_obj_size.width <<"  height: "<<min_obj_size.height<<endl;
    //cout << "Run detection at max width: "<<max_obj_size.width <<"  height: "<<max_obj_size.height<<endl;
    
    start_time = clock();
    myClassifier.detectMultiScale(img, result_list,1.02,3,0,min_obj_size,max_obj_size);
    end_time = clock();
    total_time = (float)(end_time - start_time)/CLOCKS_PER_SEC;
    
    cvReleaseImage(&tempFrame);
    return result_list; 
}

