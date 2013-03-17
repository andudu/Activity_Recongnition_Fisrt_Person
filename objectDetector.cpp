//
//  objectDetector.cpp
//  FP_ADL_Detector
//
//  Created by Yahoo on 11/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "objectDetector.h"

ObjectDetector::ObjectDetector(int indicate){

    DIR *dir;
    struct dirent *ent;
    string file_name;
    
    int counter = 0;
    int limit = -1;

    //Reade the mean std file first
    mean_std_reader();

    if ((dir = opendir (HAAR_PATH)) != NULL) {
        //print all the files and directories within directory
        while ((ent = readdir (dir)) != NULL) {
           
            file_name.assign(ent->d_name);
    
            if(file_name.compare(".") == 0 || file_name.compare("..") == 0){
                continue; 
            }
            
            //Limit the number of detectors
            if(limit!= -1 && counter == limit)break;   
            
            
            if(indicate!=-1){
                //Indicate the only one I want to evaluate
                if (counter == indicate){
                    string path = HAAR_PATH;
                    path.append(file_name);
                    
                    Haar_cascade tmp_classifier = Haar_cascade(path , file_name);
                    myHaars.push_back(tmp_classifier);
                }                
            }else{
                //Evaluate all the obj detectors
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

ObjectDetector::ObjectDetector(){

    DIR *dir;
    struct dirent *ent;
    string file_name;
    
    int counter = 0;
    int limit = -1;

    if ((dir = opendir (HAAR_PATH)) != NULL) {
        //print all the files and directories within directory
        while ((ent = readdir (dir)) != NULL) {
           
            file_name.assign(ent->d_name);
    
            if(file_name.compare(".") == 0 || file_name.compare("..") == 0){
                continue; 
            }
            
            //Limit the number of detectors
            if(limit!= -1 && counter == limit)break;               
            
            string path = HAAR_PATH;
            path.append(file_name);
            
            Haar_cascade tmp_classifier = Haar_cascade(path , file_name);
            myHaars.push_back(tmp_classifier);

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

bool ObjectDetector::mean_std_reader(){

    string line;
    ifstream myfile ("cascade_0305/mean_std.txt");
    if (myfile.is_open())
    {
        while ( myfile.good() )
        {   
            mean_std tmp;
            getline (myfile,line);
            cout << line << endl;

            typedef vector< string > split_vector_type;
            
               
            split_vector_type SplitVec;
            split( SplitVec, line, is_any_of(" ") );
            
            tmp.classifier_name = SplitVec[0];

            /*
            tmp.width_mean = atoi(SplitVec[1].c_str());
            tmp.width_std = atoi(SplitVec[2].c_str());
            tmp.height_mean = atoi(SplitVec[3].c_str());
            tmp.height_std = atoi(SplitVec[4].c_str());
            
            /*
            cout << "1\n";
            mean_std_list.push_back(tmp);
            cout << "list size:" << mean_std_list.size() << endl;
            cout << "2\n";
            */
        }
        
        myfile.close();
        
    }else{
         cout << "Unable to open mean_std file!\n";
         return false;    
    }
    
/*
    for (int i = 0 ; i < mean_std_list.size() ; i ++){
        cout << "i:" << i << endl;
        cout << mean_std_list[i].classifier_name << " " << mean_std_list[i].width_mean << " " 
             << mean_std_list[i].width_std << " " << mean_std_list[i].height_mean << " "
             << mean_std_list[i].height_std <<endl;
    }
*/  
    return true;
}