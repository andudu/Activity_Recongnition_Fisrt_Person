//
//  haar_training.cpp
//  FP_ADL_Detector
//
//  Created by Yahoo on 12/13/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "haar_cascade.h"

vector<string> Haar_cascade::reader(string path){

    string line;
    ifstream myfile (path.c_str());
    vector<string> list;
    if (myfile.is_open())
    {
        while ( myfile.good() )
        {   
            getline (myfile,line);
            
            /*
            注意最後一行的問題
            可能會把最後空行讀進來!
            所以要做長度檢查
            */
            if(line.size() == 0)
                break;

            list.push_back(line);
        }
        
        myfile.close();
        
    }else{
         cout << "Unable to open the file!\n";    
    }

    return list;
}

void Haar_cascade::min_max_reader(){

    typedef vector< string > split_vector_type;
    vector<string> list = reader("cascade_0528/mean_std.txt");

    for(int i = 0 ; i < list.size() ; i ++){
        
        split_vector_type SplitVec;
        split( SplitVec, list[i], is_any_of(" ") );

        string classifier_name = SplitVec[0];

        if (classifier_name.compare(name) == 0){
            int min = atoi(SplitVec[1].c_str());
            int max = atoi(SplitVec[2].c_str());
            min_obj_size.height = min - 50;
            min_obj_size.width = min - 50;
            max_obj_size.height = max + 50;            
            max_obj_size.width = max + 50;
        }        
    }
}

Haar_cascade::Haar_cascade(string classifier_name , string feature_name){
    
    name = feature_name;

    // Create classifier object 
    if (myClassifier.load(classifier_name))
        cout << classifier_name << " is loaded\n";
    else
        cout << "Failed loading" << classifier_name <<endl;

    min_max_reader();    
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
    //cout << "name: " << name << endl;
    //cout << "Run detection at min width: "<<min_obj_size.width <<"  height: "<<min_obj_size.height<<endl;
    //cout << "Run detection at max width: "<<max_obj_size.width <<"  height: "<<max_obj_size.height<<endl;
    
    start_time = clock();
    myClassifier.detectMultiScale(img, result_list,1.02,3,0,min_obj_size,max_obj_size);
    end_time = clock();
    total_time = (float)(end_time - start_time)/CLOCKS_PER_SEC;
    
    cvReleaseImage(&tempFrame);
    return result_list; 
}

