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
    //classifier_name = "mug_cascade.xml";

    // Create classifier object 
    if (myClassifier.load(classifier_name))
        cout << classifier_name << " is loaded\n";
    else
        cout << "Failed loading" << classifier_name <<endl;
}

Haar_cascade::~Haar_cascade(){
}

string Haar_cascade::get_name(){
    
    return name;
}

int Haar_cascade::detect(string img,int min,int max){
    
        
    //cout << name <<">>Detection begins..."<<endl;
    
    min_obj_size.height = min_obj_size.width = min;
    max_obj_size.height = max_obj_size.width = max;
    IplImage* pImg = cvLoadImage( img.c_str(), 1);
    //cout << "Image loaded..." <<endl;
    //cout << "Running detection..." <<endl;
    int num_of_detection = (int)runDetection(pImg).size();  
    cvReleaseImage( &pImg );
    //cout << "Image released..." <<endl;
    
    return num_of_detection;
}

vector<Rect> Haar_cascade::detect(IplImage* image_detect,int min,int max){
    
    
    //cout << "Detection begins..." <<endl;
    
    min_obj_size.height = min_obj_size.width = min;
    max_obj_size.height = max_obj_size.width = max;
    
    //int num_of_detection = (int)runDetection(image_detect).size();  
    
    
    return runDetection(image_detect);
}

vector<Rect> Haar_cascade::runDetection(IplImage* image_detect)  
{   
    clock_t start_time, end_time;
    float total_time = 0;
    
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
    /*
    cvNamedWindow("Origin Image to be detect", 1);  
    imshow("Origin Image to be detect", img);  
    cvWaitKey(0);  
    cvDestroyWindow("Origin Image to be detect");
     */
    //The list conaining detection results
    vector<Rect> result_list;
    
    //Run detection
    //cout << "Run detection at min_size:"<< min_obj_size.width <<" max_size:"<<max_obj_size.width<<endl;
    start_time = clock();
    myClassifier.detectMultiScale(img, result_list,1.02,3,0,min_obj_size,max_obj_size);
    end_time = clock();
    total_time = (float)(end_time - start_time)/CLOCKS_PER_SEC;
    //cout << CLOCKS_PER_SEC << endl;
    cout << "Detection took:" << total_time << " seconds\n";
    
    //cout << "Number of detections:" << result_list.size() << endl;
    
    /*
     show result only when we got detection
     
    if(result_list.size() > 0){
        
        for (int i = 1 ; i <= result_list.size() ; i++){     
            cout << "drawing bboxes:" << i << endl;
            CvPoint point1, point2;  
            
            point1.x = result_list[i].x;  
            point2.x = result_list[i].x + result_list[i].width;  
            point1.y = result_list[i].y;  
            point2.y = result_list[i].y + result_list[i].height;  
            //cout << "x1:" << point1.x << " x2:" << point2.x << " y1:"<< point1.y << " y2:" <<point2.y <<endl;
            cvRectangle(tempFrame, point1, point2, CV_RGB(0,255,0), 3, 8, 0); 
        }  
        
        IplImage *dst = 0;       
        float scale = 0.5; 
        CvSize dst_cvsize;      
        dst_cvsize.width = tempFrame->width * scale;
        dst_cvsize.height = tempFrame->height * scale;
        dst = cvCreateImage( dst_cvsize, tempFrame->depth, tempFrame->nChannels);
        cvResize(tempFrame, dst, CV_INTER_LINEAR);
        
        
         // Show the result in the window.  
         cvNamedWindow("Obj Detection Result", CV_WINDOW_AUTOSIZE);
         cvShowImage("Obj Detection Result", dst);  
         cvWaitKey(0);  
         cvDestroyWindow("Obj Detection Result");  

    }
     */
    cvReleaseImage(&tempFrame);
    return result_list;  
}  
