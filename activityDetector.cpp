//
//  activityDetection.cpp
//  FP_ADL_Detector
//
//  Created by Yahoo on 11/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "activityDetector.h"

vector<string> ActivityDetector::reader(string path){

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

vector<string> ActivityDetector::run_crf(TemporalPyramid *my_pyramid,int level, int node){
    
    typedef vector< string > split_vector_type;
    FILE* fp; //Output file for CRF++
    vector<string> activity_detected;
    
    float thres = my_pyramid->frame_per_node / thres_factor; //This is critical

    fp = fopen("crf/test.crf", "w");

    vector<int> features;

    
    for (int i = 0 ; i < my_pyramid->num_of_features ; i++){
        if(my_pyramid->pyramid[level][node].feature[i] >= thres){
            fprintf(fp, "%d ",1);
        }else{
            fprintf(fp, "%d ",0);
        }
    }
    
    fprintf(fp, "\n");

    fclose(fp);
    
    //Run CRF++
    string cmd = "crf_test -v1 -m " + crf_model + " crf/test.crf > crf/result.txt";
    system(cmd.c_str());    

    vector<string> file_list = reader("crf/result.txt");
    split_vector_type SplitVec;
    split( SplitVec, file_list[1], is_any_of("\t/") );

    activity_detected.push_back(*(SplitVec.rbegin() + 1)); //The second last element is the action get from CRF
    activity_detected.push_back(SplitVec.back()); //The last element is the corresponding prob
    
    return activity_detected;
}

vector<string> ActivityDetector::run_crf(TemporalPyramid *my_pyramid, int level_1, int node_1, int level_2, int node_2){
    
    typedef vector< string > split_vector_type;
    FILE* fp; //Output file for CRF++
    vector<string> activity_detected;
    
    float thres = my_pyramid->frame_per_node / thres_factor; //This is critical

    fp = fopen("crf/test.crf", "w");

    
    for (int i = 0 ; i < my_pyramid->num_of_features ; i++){
        if(my_pyramid->pyramid[level_1][node_1].feature[i] >= thres){
            fprintf(fp, "%d ",1);
        }else{
            fprintf(fp, "%d ",0);
        }
    }

    fprintf(fp, "\n");

    for (int i = 0 ; i < my_pyramid->num_of_features ; i++){
        if(my_pyramid->pyramid[level_2][node_2].feature[i] >= thres){
            fprintf(fp, "%d ",1);
        }else{
            fprintf(fp, "%d ",0);
        }
    }
    

    fprintf(fp, "\n");

    fclose(fp);
    
    //Run CRF++
    string cmd = "crf_test -v1 -m " + crf_model + " crf/test.crf > crf/result.txt";
    system(cmd.c_str());    

    vector<string> file_list = reader("crf/result.txt");
    split_vector_type SplitVec;
    split( SplitVec, file_list[1], is_any_of("\t/") );

    activity_detected.push_back(*(SplitVec.rbegin() + 1)); //The second last element is the action get from CRF
    activity_detected.push_back(SplitVec.back()); //The last element is the corresponding prob
    
    split( SplitVec, file_list[2], is_any_of("\t/") );

    activity_detected.push_back(*(SplitVec.rbegin() + 1)); //The second last element is the action get from CRF
    activity_detected.push_back(SplitVec.back()); //The last element is the corresponding prob

    return activity_detected;
}

bool ActivityDetector::activity_detect_cvpr_12(TemporalPyramid *my_pyramid){

    /*
    讀取最新2個node並建立2level feature, 再丟去crf
    */

    return true;
}


bool ActivityDetector::activity_detect(TemporalPyramid *my_pyramid){
 
    vector<string> activity_detected;
    my_pyramid->current_prediction.clear();
    prediction_unit tmp_prediction;
    
    for(int level = 0 ; level < my_pyramid->num_of_levels ; level++){
        
        int node = my_pyramid->pyramid[level].size() - 1;

        //Skip if this node's table has already been filled
        if(my_pyramid->pyramid[level][node].table_filled){
            continue;
        }

        //If this is a brand new node
        if(my_pyramid->pyramid[level][node].table[0][0].activity.compare("NULL") == 0){
            
            activity_detected = run_crf(my_pyramid,level,node);

            //cout << " level: "<<level<<" node : " << node << " activity: "<< activity_detected[0] << "/" << activity_detected[1] <<endl;
            my_pyramid->pyramid[level][node].table[0][0].activity = activity_detected[0];
            my_pyramid->pyramid[level][node].table[0][0].prob = atof(activity_detected[1].c_str());

            tmp_prediction.level = level;
            tmp_prediction.node = node;
            tmp_prediction.table_row = 0;
            tmp_prediction.table_col = 0;

            my_pyramid->current_prediction.push_back(tmp_prediction);
        }
        
        //If this node's 2 stage activity is not yet detected and Not the first node in this level
        if(node != 0 && my_pyramid->pyramid[level][node].table[1][0].activity.compare("NULL") == 0){

            float max_prob = -1;

            for(int level_before = 0 ; level_before < my_pyramid->num_of_levels ; level_before ++ ){
                for(int node_before = 0 ; node_before < my_pyramid->pyramid[level_before].size() ; node_before ++){
                    
                    //Skip abandoned nodes
                    if(my_pyramid->pyramid[level_before][node_before].abandoned){
                        continue;
                    }                            

                    //Skip the nodes produced not before this node
                    if(level_before < level){

                        if(node_before >= node*(pow( 2, (float)level - level_before)))
                            continue;

                    }else if(level_before == level){

                        if(node_before >= node)
                            continue;

                    }else{

                        if(node_before*(pow( 2, (float)level_before - level)) >= node)
                            continue;

                    }

                    //Runs 2 stage crf prediction and get the highest scored prediction
                    activity_detected = run_crf(my_pyramid,level_before,node_before,level,node);

                    if(atof(activity_detected[3].c_str()) > max_prob){
                        
                        my_pyramid->pyramid[level][node].table[1][0].activity = activity_detected[0];
                        my_pyramid->pyramid[level][node].table[1][0].prob = atof(activity_detected[1].c_str());
                        my_pyramid->pyramid[level][node].table[1][1].activity = activity_detected[2];
                        my_pyramid->pyramid[level][node].table[1][1].prob = atof(activity_detected[3].c_str());
                        
                        max_prob = atof(activity_detected[3].c_str());
                    }
                }
            }


            tmp_prediction.level = level;
            tmp_prediction.node = node;
            tmp_prediction.table_row = 1;
            tmp_prediction.table_col = 0;

            my_pyramid->current_prediction.push_back(tmp_prediction);

            tmp_prediction.level = level;
            tmp_prediction.node = node;
            tmp_prediction.table_row = 1;
            tmp_prediction.table_col = 1;

            my_pyramid->current_prediction.push_back(tmp_prediction);

            my_pyramid->pyramid[level][node].table_filled = true;
        }
    }    

    return true;
}

bool ActivityDetector::activity_detect_my_data(TemporalPyramid *my_pyramid){
 
    vector<string> activity_detected;
    my_pyramid->current_prediction.clear();
    prediction_unit tmp_prediction;
    
    for(int level = 0 ; level < my_pyramid->num_of_levels ; level++){
        
        int node = my_pyramid->pyramid[level].size() - 1;

        //Skip if this node's table has already been filled
        if(my_pyramid->pyramid[level][node].table_filled){
            continue;
        }

        //If this is a brand new node
        if(my_pyramid->pyramid[level][node].table[0][0].activity.compare("NULL") == 0){
            
            activity_detected = run_crf(my_pyramid,level,node);

            //cout << " level: "<<level<<" node : " << node << " activity: "<< activity_detected[0] << "/" << activity_detected[1] <<endl;
            my_pyramid->pyramid[level][node].table[0][0].activity = activity_detected[0];
            my_pyramid->pyramid[level][node].table[0][0].prob = atof(activity_detected[1].c_str());

            tmp_prediction.level = level;
            tmp_prediction.node = node;
            tmp_prediction.table_row = 0;
            tmp_prediction.table_col = 0;

            my_pyramid->current_prediction.push_back(tmp_prediction);
        }
        
        //If this node's 2 stage activity is not yet detected and Not the first node in this level
        if(node != 0 && my_pyramid->pyramid[level][node].table[1][0].activity.compare("NULL") == 0){

            float max_prob = -1;

            for(int level_before = 0 ; level_before < my_pyramid->num_of_levels ; level_before ++ ){
                for(int node_before = 0 ; node_before < my_pyramid->pyramid[level_before].size() ; node_before ++){
                    
                    //Skip abandoned nodes
                    if(my_pyramid->pyramid[level_before][node_before].abandoned){
                        continue;
                    }                            

                    //Skip the nodes produced not before this node
                    if(level_before < level){

                        if(node_before >= node*(pow( 2, (float)level - level_before)))
                            continue;

                    }else if(level_before == level){

                        if(node_before >= node)
                            continue;

                    }else{

                        if(node_before*(pow( 2, (float)level_before - level)) >= node)
                            continue;

                    }

                    //Runs 2 stage crf prediction and get the highest scored prediction
                    activity_detected = run_crf(my_pyramid,level_before,node_before,level,node);

                    if(atof(activity_detected[3].c_str()) > max_prob){
                        
                        my_pyramid->pyramid[level][node].table[1][0].activity = activity_detected[0];
                        my_pyramid->pyramid[level][node].table[1][0].prob = atof(activity_detected[1].c_str());
                        my_pyramid->pyramid[level][node].table[1][1].activity = activity_detected[2];
                        my_pyramid->pyramid[level][node].table[1][1].prob = atof(activity_detected[3].c_str());
                        
                        max_prob = atof(activity_detected[3].c_str());
                    }
                }
            }


            tmp_prediction.level = level;
            tmp_prediction.node = node;
            tmp_prediction.table_row = 1;
            tmp_prediction.table_col = 0;

            my_pyramid->current_prediction.push_back(tmp_prediction);

            tmp_prediction.level = level;
            tmp_prediction.node = node;
            tmp_prediction.table_row = 1;
            tmp_prediction.table_col = 1;

            my_pyramid->current_prediction.push_back(tmp_prediction);

            my_pyramid->pyramid[level][node].table_filled = true;
        }
    }    

    return true;
}

bool ActivityDetector::print_info(string info_id){

    if(info_id.compare("num_of_levels") == 0){
        cout << "num_of_levels:" << "test"<< endl;
        return true;
    }

    return false;
}

ActivityDetector::ActivityDetector(int thres, string _crf_model, bool _build_pyramid){
    crf_model = _crf_model;    
    thres_factor = (float)thres;
    build_pyramid = _build_pyramid;
}

ActivityDetector::~ActivityDetector(){
}


