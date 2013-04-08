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

bool TemporalPyramid::loadFrames_realtime(FrameModel* frames, int frame_index){

    if( frame_index == 0){
        //Initialize if it is the first frame.
        //Clear the pyramid first
        pyramid.clear();

        //Setting the 'frame per node' number
        frame_per_node = frames->FPS;
        num_of_features = frames->num_features;
    
        //this->print_info("frame_per_node");
        //frames->print_info("num_frames");

        //Build the first level pyramid
        vector<node> tmp_node_array;

        pyramid.push_back(tmp_node_array);
        num_of_levels = (int)pyramid.size();

    }else{
        int sliding_window_start = frame_index - frame_per_node;
        
        //create a tmp node with the same number of features in a frame
        node tmp_node;
        
        for (int i = 0 ; i < frames->num_features ; i++){
            //cout << "frame_model->frameList["<<f<<"].feature.size()" << frames->frameList[f].feature.size() <<endl;
            //cout << frames->frameList[f].feature.size() <<endl;
            tmp_node.feature.push_back(0);
        }
        
        //Summing frame features in an interval(FPN)
        for (int j = sliding_window_start; j < sliding_window_start + frame_per_node ; j++) {
            for (int i = 0 ; i < frames->num_features ; i++){
                //cerr << " j:"<< j <<" i:" << i << endl;
                //tmp_node.feature[i] = frames->frameList[j].feature[i] + tmp_node.feature[i];
                tmp_node.feature[i] = frames->frameList[j].feature[i] + tmp_node.feature[i];
            } 
        }

        //Similarity check with the latest node
        bool similar = true;
        if ( pyramid[0].size() >= 1){
            for (int i = 0 ; i < frames->num_features ; i++){
                float diff =  abs(pyramid[0].back().feature[i] - tmp_node.feature[i]);
                
                if(diff/frame_per_node > CORRELATION_THRES){
                    similar = false;
                    break;
                }
            }
        }else{
            similar = false;
        }

        if(!similar){
            pyramid[0].push_back(tmp_node);
            return true;
        }else{
            return false;
        }
        
    }
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

        cout << "node " << node << " : ";

        if(pyramid[level_index][node].abandoned){
            cout << "This is an abandoned node!\n\n";
            continue;
        }

        for (int f = 0; f < pyramid[level_index][node].feature.size() ; f++) {
            cout << pyramid[level_index][node].feature[f] <<" "; 
        }

        cout << " | " << endl;
        cout << "activity as 1 stage action : " << pyramid[level_index][node].table[0][0].activity << " / " << pyramid[level_index][node].table[0][0].prob << endl << endl;

        if(node != 0){
            cout << "activity as 2 stages action: " << pyramid[level_index][node].table[1][0].activity << " / " << pyramid[level_index][node].table[1][0].prob << " , ";
            cout << pyramid[level_index][node].table[1][1].activity << " / " << pyramid[level_index][node].table[1][1].prob << endl << endl;
        }
    }
    
    return true;
}

bool  TemporalPyramid::showCurrentPrediction(){
    
    cout << "=================================\n";

    for(int i = 0 ; i < current_prediction.size() ; i++){

        node tmp_node = pyramid[current_prediction[i].level][current_prediction[i].node];
        int row = current_prediction[i].table_row;
        int col = current_prediction[i].table_col;

        cout << "Level: " << current_prediction[i].level <<" node: " <<  current_prediction[i].node << endl ;
        cout << "table_row: " << row << " table_col: " << col <<endl;
        cout << tmp_node.table[row][col].activity <<" / " <<  tmp_node.table[row][col].prob << endl << endl;
    }  
    
    
    return true;
}


bool TemporalPyramid::buildPyramid_realtime(){

    int level_required;
    int pyramid_base_size = pyramid[0].size();

    cout << "pyramid_base_size:" << pyramid_base_size <<endl;
    if((pyramid_base_size % 2 )!= 0){
        level_required = (int)log2(pyramid_base_size);
    }else{
        level_required = (int)log2(pyramid_base_size) + 1;
    }

    if (level_required <= 1) {
        cout << "Invalid number for levels\n";
        return false;
    }
    
    cout << "level to be built:" << level_required <<endl;
    
    //level in pyramid start from 0
    for (int level = 1 ;  level < level_required ; level++){
            
        //The first node in this level
        if( pyramid.size() < level + 1){
            vector<node> tmp_node_array;
            pyramid.push_back(tmp_node_array);
        }


        int nodes_already_in_this_level = pyramid[level].size();

        for(int n = nodes_already_in_this_level*2 ; n < pyramid[level-1].size() ; n = n + 2){
            
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
            
            pyramid[level].push_back(tmp_node);
        }
    }
    
    num_of_levels = (int)pyramid.size();
    
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
    
    cout << "level to be built:" << level_required <<endl;

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

bool abandon_decision(int index, int length){

    float abandoned_coeff = 0.3;
    //cout << "\n\n\n\n" <<(float)(index+1)/length << "\n\n\n\n";
    if( (float)(index+1)/length  < abandoned_coeff){
        return true;
    }else{
        return false;
    }

}

bool TemporalPyramid::refreshPyramid_realtime(){
    
    for(int level = 0 ; level < num_of_levels ; level++){
        for(int node = 0 ; node < pyramid[level].size() ; node ++){
            if ( abandon_decision(node, pyramid[level].size())){
                pyramid[level][node].abandoned = true;
            }
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

    if(info_id.compare("current_prediction") == 0){
            showCurrentPrediction();
        return true;
    }

    return false;
}






