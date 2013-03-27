//
//  temporalPyramid.cpp
//  FP_ADL_Detector
//
//  Created by Yahoo on 11/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "temporalPyramid.h"

vector<string> split(string& str,const char* c)
{
    char *cstr, *p;
    vector<string> res;
    cstr = new char[str.size()+1];
    strcpy(cstr,str.c_str());
    p = strtok(cstr,c);
    while(p!=NULL)
    {
        res.push_back(p);
        p = strtok(NULL,c);
    }
    return res;
}


TemporalPyramid::TemporalPyramid(){
}

TemporalPyramid::~TemporalPyramid(){
}

bool TemporalPyramid::observationSampling(){
    
    return true;
}

bool TemporalPyramid::loadFrames_realtime(FrameModel* frames){
    
    int sliding_window_start = 0;

    //Clear the pyramid first
    pyramid.clear();
   
    //Setting the 'frame per node' number
    frame_per_node = frames->FPS;
    num_of_features = frames->num_features;
    
    //this->print_info("frame_per_node");
    //frames->print_info("num_frames");

    //Build the first level pyramid
    vector<node> tmp_node_array;

    //Abandon earlier frames
    //暫定最多2^5的第一層node即可(已註解)
    /*
    if(frames->num_frames >= 32*frame_per_node)
        sliding_window_start = frames->num_frames - 32*frame_per_node;
    */

    //Building pyramid ,level 1
    //stepping node by node
    for(int f = sliding_window_start ; f + frame_per_node < frames->num_frames ; f = f + frame_per_node){
        //create a tmp node with the same number of features in a frame
        node tmp_node;
        for (int i = 0 ; i < frames->num_features ; i++){
            //cout << "frame_model->frameList["<<f<<"].feature.size()" << frames->frameList[f].feature.size() <<endl;
            //cout << frames->frameList[f].feature.size() <<endl;
            tmp_node.feature.push_back(frames->frameList[f].feature[i]);
        }
        //Summing frame features in an interval(FPN)
        for (int j = f+1; j < f + frame_per_node ; j++) {
            for (int i = 0 ; i < frames->num_features ; i++){
                //cerr << "f:" << f <<" j:"<< j <<" i:" << i << endl;
                tmp_node.feature[i] = frames->frameList[j].feature[i] + tmp_node.feature[i];
            } 
        }
        //push it into the temp node array
        tmp_node_array.push_back(tmp_node);
    }
    
    //push it to the pyramid, level 1.    
    pyramid.push_back(tmp_node_array);
    num_of_levels = (int)pyramid.size();

    return true;
}

bool TemporalPyramid::loadFrames(FrameModel* frames){
    
    //Clear the pyramid first
    pyramid.clear();
    
    //Setting the 'frame per node' number
    //frame_per_node = FPN;
    frame_per_node = frames->FPS;
    num_of_features = frames->num_features;
    
    cout << "FPN: " << frame_per_node << endl;
    //Build the first level pyramid
    vector<node> tmp__node_array;
    for(int f = 0 ; f + frame_per_node < frames->frame_count ; f = f + frame_per_node){
        
        //create a node with the same number of features in a frame
        node tmp_node;
        for (int i = 0 ; i < frames->num_features ; i++){
            tmp_node.feature.push_back(frames->frameList[f].feature[i]);
        }
        
        //Summing frame features in a interval(FPN)
        for (int j = f+1; j < f + frame_per_node ; j++) {
            for (int i = 0 ; i < frames->num_features ; i++){
                //cerr << "f:" << f <<" j:"<< j <<" i:" << i << endl;
                tmp_node.feature[i] = frames->frameList[j].feature[i] + tmp_node.feature[i];
            } 
        }
        
        tmp__node_array.push_back(tmp_node);
    }
    
        
    pyramid.push_back(tmp__node_array);
    num_of_levels = (int)pyramid.size();
    
    return true;
}

bool  TemporalPyramid::showPyramid(int level_index){
    
    if ( level_index >= pyramid.size()) {
        cout << "failed\n";
        return false;
    }
    
    cout << "=================================\n";
    cout << "Level:" << level_index <<" node:" <<  pyramid[level_index].size() << endl;
    for (int i = 0 ;  i < pyramid[level_index].size(); i++) {
        for (int j = 0; j < pyramid[level_index][i].feature.size() ; j++) {
            cout << pyramid[level_index][i].feature[j] <<" "; 
        }
        cout << " | ";
    }
    cout << "\n";
    
    return true;
}

bool TemporalPyramid::buildPyramid(int frame_size ,int FPS){

    int level_required;

    cout << "frameList.size():" << frame_size <<endl;
    if((frame_size/FPS % 2 )!= 0){
        level_required = (int)log2(frame_size/FPS);
    }else{
        level_required = (int)log2(frame_size/FPS) + 1;
    }        

    if (num_of_levels > 1) {
        cout << "Pyramid already built. Use 'loadFrames' to re-initial the pyramid first.\n";
        return false;
    }
    
    if (level_required <= 1) {
        cout << "Invalid number for levels\n";
        return false;
    }
    
    cout << "level required:" << level_required <<endl;

    for (int level = 1 ;  level < level_required ; level++) {
        
        vector<node> tmp__node_array;
        for(int n = 0 ; n < pyramid[level-1].size() ; n = n + 2){
            
            //create a node with the same number of features
            node tmp_node;
            for (int i = 0 ; i < num_of_features ; i++){
                tmp_node.feature.push_back(0);
            }
            
            //In case the number of nodes in the last level is not even
            if (n+1 >= pyramid[level-1].size()) {
                break;
            }else{               
                //Summing node features in a interval and avrage them(2 nodes)
                for (int k = 0; k < num_of_features; k++) {
                    tmp_node.feature[k] = (tmp_node.feature[k] + pyramid[level-1][n+1].feature[k])/2;
                }
            }            
            
            tmp__node_array.push_back(tmp_node);
        }
        
        pyramid.push_back(tmp__node_array);
    }
    
    num_of_levels = (int)pyramid.size();
    
    return true;
}

string TemporalPyramid::run_crf(FrameModel *frames, int level){
    
    FILE* fp; //Output file for CRF++
    vector<string> feature_names;
    string line;
    vector<string> tmp;
    
    //run through the nodes in the level 
    for (int node = 0 ;  node < pyramid[level].size(); node++) {
        
        float max = 0;
        string feature_name = "";
        
        //run through the features in the node
        for (int _feature = 0; _feature < pyramid[level][node].feature.size() ; _feature++) {
            
            if (pyramid[level][node].feature[_feature] > max && pyramid[level][node].feature[_feature] > frame_per_node/3) {
                max = pyramid[level][node].feature[_feature];
                feature_name = frames->feature_name[_feature];
            }
        }
        
        feature_names.push_back(feature_name);
    }
    
    //Node number checking
    if(feature_names.size() < 3){
        char msg[] = "Not enough nodes in level";
        sprintf(msg, "Failed: %s %d !", msg, level);
        return  msg;
    }
    
    cout << "Features: ";
    /*
    for (int i  = 0 ;  i < feature_names.size() ; i ++) {
        cout << feature_names[i] << " ";
    }
    */
    cout << feature_names[feature_names.size()-3] << ", " << feature_names[feature_names.size()-2] << ", " << feature_names[feature_names.size()-1] << endl;
    fp = fopen("crf/test.crf", "w");
    
    fprintf(fp, "%s\n",feature_names[feature_names.size()-3].c_str());
    fprintf(fp, "%s\n",feature_names[feature_names.size()-2].c_str());
    fprintf(fp, "%s\n",feature_names[feature_names.size()-1].c_str());
    
    fclose(fp);
    
    //Run CRF++
    system("crf_test -m crf/model.crf crf/test.crf > crf/result.txt");
    
    //Read result 
    ifstream myfile("crf/result.txt");
    if (myfile.is_open())
    {   
        //cout << "Reading result from CRF++\n";
        while ( myfile.good() )
        {
            getline (myfile,line);
            tmp.push_back(line);
            //cout << line <<endl;
        }
        
    }else {
        cout << "Unable to open file";
    }
    
    myfile.close();

    typedef vector< string > split_vector_type;
    
    split_vector_type SplitVec;
    split( SplitVec, tmp[tmp.size() - 3 ], is_any_of("\t") ); 
    
    return SplitVec[1];
}

bool TemporalPyramid::activity_detect(FrameModel *frames, int min_num_act_seq){
    string result;
    for(int level = 0 ; level < num_of_levels ; level++){
        if(pyramid[level].size() >= min_num_act_seq){
            cout << "Level:" << level << endl;
            result = run_crf(frames, level);
            cout << "\nResult from CRF++:"  << result <<endl<<endl;
            
        }else{
            cout << "Level:" << level << " Not enough nodes" <<endl;
        }
        
    }
        
    return true;
}

bool TemporalPyramid::print_info(string info_id){

    if(info_id.compare("num_of_levels") == 0){
        cout << "num_of_levels:" << num_of_levels << endl;
        return true;
    }

    if(info_id.compare("frame_per_node") == 0){
        cout << "frame_per_node:" << frame_per_node << endl;
        return true;
    }

    if(info_id.compare("pyramid") == 0){
        for(int l = 0 ; l < num_of_levels ; l++){
            showPyramid(l);
        }            
        return true;
    }

    return false;
}






