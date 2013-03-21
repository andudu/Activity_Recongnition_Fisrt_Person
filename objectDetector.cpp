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
    
            if(file_name.compare(".") == 0 || file_name.compare("..") == 0 ||
               file_name.compare("mean_std.txt") == 0){
                continue; 
            }
            
            //Limit the number of detectors
            if(limit!= -1 && counter == limit)break;   
            

            int width_mean=0,height_mean=0,width_std=0,height_std=0;
            for(int i=0;i<mean_std_list.size();i++){
                if(mean_std_list[i].classifier_name.compare(file_name)){
                    width_mean = mean_std_list[i].width_mean;
                    height_mean = mean_std_list[i].height_mean;
                    width_std = mean_std_list[i].width_std;
                    height_std = mean_std_list[i].height_std;
                }
            }
            
            if(indicate!=-1){
                //Indicate the only one I want to evaluate
                if (counter == indicate){
                    string path = HAAR_PATH;
                    path.append(file_name);
                    
                    Haar_cascade tmp_classifier = Haar_cascade(path , file_name,width_mean,height_mean,width_std,height_std);
                    myHaars.push_back(tmp_classifier);
                }                
            }else{
                //Evaluate all the obj detectors
                string path = HAAR_PATH;
                path.append(file_name);

                Haar_cascade tmp_classifier = Haar_cascade(path , file_name,width_mean,height_mean,width_std,height_std);
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
        vector<Rect> result_list = myHaars[cls].detect(image);
        frame_model->frameList[frame_index].feature.push_back(result_list.size());
        frame_model->frameList[frame_index].result_list.push_back(result_list);
        
        
    }
    
    return true;
}

vector<string> reader(){

    string line;
    ifstream myfile ("cascade_0305/mean_std.txt");
    vector<string> list;
    if (myfile.is_open())
    {
        while ( myfile.good() )
        {   
            /*
            注意最後一行的問題
            可能會把最後空行讀進來!
            需要做最後換行符檢查
            */
            if(line.compare("\n") == 0)
                break;
            
            getline (myfile,line);
            cout << line << endl;
            
            list.push_back(line);
        }
        
        myfile.close();
        
    }else{
         cout << "Unable to open mean_std file!\n";    
    }

    return list;
}

bool ObjectDetector::mean_std_reader(){

    typedef vector< string > split_vector_type;

    vector<string> list = reader();

    for(int i = 0 ; i < list.size() ; i ++){
        mean_std tmp;
        
        split_vector_type SplitVec;
        split( SplitVec, list[i], is_any_of(" ") );
        
        tmp.classifier_name = SplitVec[0];
        tmp.width_mean = atoi(SplitVec[1].c_str());
        tmp.width_std = atoi(SplitVec[2].c_str());
        tmp.height_mean = atoi(SplitVec[3].c_str());
        tmp.height_std = atoi(SplitVec[4].c_str());
        
        mean_std_list.push_back(tmp);
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

