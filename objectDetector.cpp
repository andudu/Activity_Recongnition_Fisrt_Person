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
    
    int counter = 1;

    
    //Reade the mean std file first
    //mean_std_reader();

    if ((dir = opendir (HAAR_PATH)) != NULL) {
        //print all the files and directories within directory
        while ((ent = readdir (dir)) != NULL) {
           
            file_name.assign(ent->d_name);
            
            cout << "cascade file name:" << file_name << endl;

            if(file_name.compare(".") == 0 || file_name.compare("..") == 0 ||
               file_name.compare("mean_std.txt") == 0){
                continue; 
            }

            /*
            int width_mean=0,height_mean=0,width_std=0,height_std=0;
            for(int i = 0 ; i < mean_std_list.size() ; i++){
                if(mean_std_list[i].classifier_name.compare(file_name) == 0){
                    width_mean = mean_std_list[i].width_mean;
                    height_mean = mean_std_list[i].height_mean;
                    width_std = mean_std_list[i].width_std;
                    height_std = mean_std_list[i].height_std;
                    break;
                }
            }
            */
            
            if(indicate!=-1){
                //Indicate the only object detector I want to evaluate
                if (counter == indicate){
                    string path = HAAR_PATH;
                    path.append(file_name);

                    //Haar_cascade tmp_classifier = Haar_cascade(path , file_name,width_mean,height_mean,width_std,height_std);
                    Haar_cascade tmp_classifier = Haar_cascade(path , file_name);
                    myHaars.push_back(tmp_classifier);
                }                
            }else{
                //Evaluate all the obj detectors
                string path = HAAR_PATH;
                path.append(file_name);

                //Haar_cascade tmp_classifier = Haar_cascade(path , file_name,width_mean,height_mean,width_std,height_std);
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
            
        //cout << "detecting:" << "/" << frame_model->frameList.size()-1 << endl;
        //cout << "detecting obj: " << cls << endl;
        vector<Rect> result_list = myHaars[cls].detect(image);

        //Consider "existence", ignore the number shown in a frame
        if(result_list.size()>=1){
            frame_model->frameList[frame_index].feature.push_back(1);
        }else{
            frame_model->frameList[frame_index].feature.push_back(0);
        }
        
        frame_model->frameList[frame_index].result_list.push_back(result_list);
        
        
    }
    
    return true;
}

/*
 Ground truth detection
*/
bool ObjectDetector::ground_truth_detect(FrameModel* frame_model , int frame_index ,IplImage* image, int frame_start){

    /*
    frame_index : the frame index in temporal pyramid level 1
    so I have to add the offset 'frame_start' to get to the corresponding frame in the video
    */
    int frame_index_pyramid = frame_index;
    int frame_index_video = frame_index + frame_start;
    bool use_cache = true;
    vector< vector<Rect> > tmp_result_list_cached;
    
    //If this is the first frame
    if( frame_index_pyramid == 0){
        
        frame_model->num_features = obj_name.size();
        
        //Fill in the feature names
        frame_model->feature_name.clear();

        for(int i = 1 ; i <= obj_name.size() ; i++){
            frame_model->feature_name.push_back(obj_name[i]);
        }            
    }

    
    ////I forgot this is for what XD. Just leave it here now.
    name_of_frames = frame_model->name;
  
    //Detection using the ground truth data
    frame_annotation annotation = ground_truth[frame_index_video];

    int feature = 0;
    //result_list_cached.clear();

    //Run through each obj
    //Beware to start from 1 to 89, not 0 to 88!!
    //Since our obj_index : obj_name pairs start from 1.
    for (int obj_index = 1; obj_index <= frame_model->num_features; obj_index ++){
         vector<Rect> result;
         
         if(annotation.objs.find(obj_index) != annotation.objs.end()){
            //The obj cls is detected in this frame
            //cout << "found!" <<endl;
            //cout << "frame_index:"<< frame_index << " obj_index:" << obj_index << " obj_name:" << annotation.objs[obj_index].name << endl;
            Rect tmp;
            tmp.x = annotation.objs[obj_index].x;
            tmp.y = annotation.objs[obj_index].y;
            tmp.width = annotation.objs[obj_index].width;
            tmp.height = annotation.objs[obj_index].height;
            result.push_back(tmp);

            //There is at least one detection in this frame.
            //we dont use cache here, use this frame as new cache instead.
            if(use_cache){
                use_cache = false;
                result_list_cached.clear();
            }
        }

        tmp_result_list_cached.push_back(result);
        frame_model->frameList[frame_index_pyramid].feature.push_back(result.size());
        frame_model->frameList[frame_index_pyramid].result_list.push_back(result); 
    }

    if(frame_index_pyramid > 0 && use_cache ){
        //use cache
        //cout << "use cache" << endl;
        //cout << "cache size:" << result_list_cached.size() <<endl;

        frame_model->frameList[frame_index_pyramid].feature.clear();
        frame_model->frameList[frame_index_pyramid].result_list.clear();

        for(int i = 0 ; i < result_list_cached.size() ; i++){            
            frame_model->frameList[frame_index_pyramid].feature.push_back(result_list_cached[i].size());
            frame_model->frameList[frame_index_pyramid].result_list.push_back(result_list_cached[i]);
            //cout <<"i:" << i << " " <<result_list_cached[i].size() <<" ";        
        }
        //cout << "frame_model->frameList["<< frame_index_pyramid<<"].feature.size " << frame_model->frameList[frame_index_pyramid].feature.size() << endl;
    }else{
        //refresh the cache
        
        result_list_cached = tmp_result_list_cached;
        //cout << "refresh the cache" << endl;
        //cout << "tmp cache size:" << tmp_result_list_cached.size() <<endl;
        //cout << "cache size:" << result_list_cached.size() <<endl;        
        
    }

    return true;
}

vector<string> ObjectDetector::reader(string path){

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

bool ObjectDetector::mean_std_reader(){

    typedef vector< string > split_vector_type;
    string path = HAAR_PATH;
    path.append("/mean_std.txt");
    vector<string> list = reader(path);

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

bool ObjectDetector::load_dpm_obj_detection(string path, float thres){

    cout << "\n\nload_dpm_obj_detection\n\n"; 

    typedef vector< string > split_vector_type;
    obj_info tmp_obj;

    //Read obj_list.txt first to get obj_name mapping
    vector<string> file_obj_list = reader("translated_with_obj_name/obj_list.txt");

    for(int i = 0 ; i < file_obj_list.size() ; i ++){
        split_vector_type SplitVec;
        split( SplitVec, file_obj_list[i], is_any_of(" ") );
        
        int tmp_obj_index = atoi(SplitVec[0].c_str());
        string tmp_obj_name = SplitVec[2];

        obj_name[tmp_obj_index] = tmp_obj_name;
    }

    //Then read the ground truth data
    vector<string> file = reader(path);

    for(int i = 0 ; i < file.size() ; i ++){

        split_vector_type SplitVec;
        split( SplitVec, file[i], is_any_of(" ") );

        /*
        tmp_obj.name = SplitVec[7];
        tmp_obj.frame = atoi(SplitVec[4].c_str());
        tmp_obj.x = atoi(SplitVec[0].c_str())*2;
        tmp_obj.y = atoi(SplitVec[1].c_str())*2;
        tmp_obj.width = atoi(SplitVec[2].c_str())*2 - atoi(SplitVec[0].c_str())*2;
        tmp_obj.height = atoi(SplitVec[3].c_str())*2 - atoi(SplitVec[1].c_str())*2;
        tmp_obj.index = atoi(SplitVec[6].c_str());
        tmp_obj.exist = true;
        */

        tmp_obj.name = SplitVec[8];
        tmp_obj.frame = atoi(SplitVec[4].c_str());
        tmp_obj.x = atoi(SplitVec[0].c_str());
        tmp_obj.y = atoi(SplitVec[1].c_str());
        tmp_obj.width = atoi(SplitVec[2].c_str()) - tmp_obj.x;
        tmp_obj.height = atoi(SplitVec[3].c_str()) - tmp_obj.y;
        tmp_obj.index = atoi(SplitVec[6].c_str());
        tmp_obj.dpm_score = atof(SplitVec[7].c_str());
        tmp_obj.exist = true;

        //Threshold
        if(tmp_obj.dpm_score < thres )
           continue;

        if(ground_truth.find(tmp_obj.frame) == ground_truth.end()){
            frame_annotation tmp;
            tmp.objs[tmp_obj.index] = tmp_obj;
            ground_truth[tmp_obj.frame] = tmp;
        }else{
            ground_truth[tmp_obj.frame].objs[tmp_obj.index] = tmp_obj;
        }            
    }

    cout << "annotation file:" << path << "is loaded."<<endl;

    return true;
}

bool ObjectDetector::load_ground_truth_obj_annotation(string path){

    cout << "\n\nload_ground_truth_obj_annotation\n\n";

    typedef vector< string > split_vector_type;
    obj_info tmp_obj;

    //Read obj_list.txt first to get obj_name mapping
    vector<string> file_obj_list = reader("translated_with_obj_name/obj_list.txt");

    for(int i = 0 ; i < file_obj_list.size() ; i ++){
        split_vector_type SplitVec;
        split( SplitVec, file_obj_list[i], is_any_of(" ") );
        
        int tmp_obj_index = atoi(SplitVec[0].c_str());
        string tmp_obj_name = SplitVec[2];

        obj_name[tmp_obj_index] = tmp_obj_name;
    }

    //cout << "\n\n\nframe_model->num_features:" << obj_name.size() <<endl;
    
    //Then read the ground truth data
    vector<string> file = reader(path);

    for(int i = 0 ; i < file.size() ; i ++){

        split_vector_type SplitVec;
        split( SplitVec, file[i], is_any_of(" ") );

        tmp_obj.name = SplitVec[7];
        tmp_obj.frame = atoi(SplitVec[4].c_str());
        tmp_obj.x = atoi(SplitVec[0].c_str())*2;
        tmp_obj.y = atoi(SplitVec[1].c_str())*2;
        tmp_obj.width = atoi(SplitVec[2].c_str())*2 - atoi(SplitVec[0].c_str())*2;
        tmp_obj.height = atoi(SplitVec[3].c_str())*2 - atoi(SplitVec[1].c_str())*2;
        tmp_obj.index = atoi(SplitVec[6].c_str());
        tmp_obj.exist = true;

        if(ground_truth.find(tmp_obj.frame) == ground_truth.end()){
            frame_annotation tmp;
            tmp.objs[tmp_obj.index] = tmp_obj;
            ground_truth[tmp_obj.frame] = tmp;
        }else{
            ground_truth[tmp_obj.frame].objs[tmp_obj.index] = tmp_obj;
        }
            
    }

    cout << "annotation file:" << path << "is loaded."<<endl;
    
    /*
    map<int, string>::iterator it;
    cout << "ground_truth:" <<endl;
    for(it = obj_name.begin() ; it != obj_name.end() ; it++)
        cout<<it->first<<" "<<it->second<<endl;
    /*
    map<string,int>::iterator it2;
    cout << "ground_truth:" <<endl;
    for(it2 = obj_name_reverse.begin() ; it2 != obj_name_reverse.end() ; it2++)
        cout<<it2->first<<" "<<it2->second<<endl;
    */

    return true;
}

