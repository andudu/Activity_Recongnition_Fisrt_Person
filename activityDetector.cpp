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
    float thres = 10.0;

    fp = fopen("crf/test.crf", "w");

    vector<int> features;

    for (int i = 0 ; i < my_pyramid->num_of_features ; i++){
        if(my_pyramid->pyramid[level][node].feature[i] >= thres){
            //features.push_back(1);
            fprintf(fp, "%d ",1);
        }else{
            //features.push_back(0);
            fprintf(fp, "%d ",0);
        }
    }

    fprintf(fp, "\n");

    fclose(fp);

    
    //Run CRF++
    system("crf_test -v1 -m crf/model_fold_1.crf crf/test.crf > crf/result.txt");
    

    vector<string> file_list = reader("crf/result.txt");
    split_vector_type SplitVec;
    split( SplitVec, file_list[1], is_any_of("\t/") );
        
    /*
    for(int i = 0 ; i < SplitVec.size() ; i ++){
        cout << " SplitVec: " << i << SplitVec[i] << endl;
    }
    */

    activity_detected.push_back(*(SplitVec.rbegin() + 1)); //The second last element is the prob get from CRF
    activity_detected.push_back(SplitVec.back()); //The last element is the corresponding activity detected
    
    return activity_detected;
}

bool ActivityDetector::activity_detect(TemporalPyramid *my_pyramid){

    
    vector<string> activity_detected;

    for(int level = 0 ; level < my_pyramid->num_of_levels ; level++){
        for (int node = 0 ; node < my_pyramid->pyramid[level].size() ; node ++){
            activity_detected = run_crf(my_pyramid,level,node);
            cout << " level: "<<level<<" node : " << node << " activity: "<< activity_detected[0] << "/" << activity_detected[1] <<endl;
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

ActivityDetector::ActivityDetector(){
}

ActivityDetector::~ActivityDetector(){
}


