//
//  temporalPyramid.cpp
//  FP_ADL_Detector
//
//  Created by Yahoo on 11/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "temporalPyramid.h"

TemporalPyramid::TemporalPyramid(){
}

TemporalPyramid::~TemporalPyramid(){
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
    cout << "Level:" << level_index <<" node:" <<  pyramid[level_index].size() << endl << endl;
    for (int node = 0 ;  node < pyramid[level_index].size(); node++) {
        for (int f = 0; f < pyramid[level_index][node].feature.size() ; f++) {
            cout << pyramid[level_index][node].feature[f] <<" "; 
        }
        cout << " | " << endl;
        cout << "activity : " << pyramid[level_index][node].activity << " / " << pyramid[level_index][node].prob << endl << endl;
    }
    
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






