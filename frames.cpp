//
//  frames.cpp
//  FP_ADL_Detector
//
//  Created by Yahoo on 11/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "frames.h"
#include "objectDetector.h"
#include "temporalPyramid.h"

vector<string> reader(string path){

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

bool FrameModel::load_ground_truth_obj_annotation(string path){

    typedef vector< string > split_vector_type;

    vector<string> file = reader(path);
    obj_info tmp_obj;

    for(int i = 0 ; i < file.size() ; i ++){

        split_vector_type SplitVec;
        split( SplitVec, file[i], is_any_of(" ") );

        tmp_obj.name = SplitVec[7];
        tmp_obj.frame = atoi(SplitVec[4].c_str());
        tmp_obj.x = atoi(SplitVec[0].c_str())*2;
        tmp_obj.y = atoi(SplitVec[1].c_str())*2;
        tmp_obj.width = atoi(SplitVec[2].c_str());
        tmp_obj.height = atoi(SplitVec[3].c_str());
        tmp_obj.index = atoi(SplitVec[6].c_str());
        tmp_obj.exist = true;

        if(obj_name.find(tmp_obj.index) == obj_name.end()){
            obj_name[tmp_obj.index] = tmp_obj.name;
            obj_name_reverse[tmp_obj.name] = tmp_obj.index;
        }
        
        if(ground_truth.find(tmp_obj.frame) == ground_truth.end()){
            frame_annotation tmp;
            tmp.objs[tmp_obj.index] = tmp_obj;
            ground_truth[tmp_obj.frame] = tmp;
        }else{
            ground_truth[tmp_obj.frame].objs[tmp_obj.index] = tmp_obj;
        }
            
    }

    
    map<int, string>::iterator it;
    cout << "ground_truth:" <<endl;
    for(it = obj_name.begin() ; it != obj_name.end() ; it++)
        cout<<it->first<<" "<<it->second<<endl;
    
    map<string,int>::iterator it2;
    cout << "ground_truth:" <<endl;
    for(it2 = obj_name_reverse.begin() ; it2 != obj_name_reverse.end() ; it2++)
        cout<<it2->first<<" "<<it2->second<<endl;

    return true;
}

float ObjectDetector_Evaluation(int frame_index,TemporalPyramid* my_pyramid){

    int level_index = 0;

    cout << "Number of nodes in level " << level_index  << " => " << my_pyramid->pyramid[level_index].size() <<endl;;
    cout << "Node features" << endl;
    for (int i = 0 ;  i < my_pyramid->pyramid[level_index].size(); i++) {
        for (int j = 0; j < my_pyramid->pyramid[level_index][i].feature.size() ; j++) {
            cout << my_pyramid->pyramid[level_index][i].feature[j] <<" "; 
        }
        cout << " | ";
    }
    cout << "\n";

    return 0;
}


int FrameModel::getFPS(){

    return FPS;
}

bool FrameModel::loadVideo_realtime(string path, bool pause_when_detected ,bool show_detection_result,int start, int end, int indicate){
    
    ObjectDetector* myObjDetector = new ObjectDetector(indicate);
    TemporalPyramid* myTemporalPyramid = new TemporalPyramid;
    CvCapture *capture;
    IplImage *frame;
    frameNode temp;

    cvNamedWindow("Obj Detection Result", CV_WINDOW_AUTOSIZE);
    cvMoveWindow("Obj Detection Result", 50, 0);
    
    ////Laoding video file
    cout << "Laoding video file\n";
    capture = cvCaptureFromAVI(path.c_str());
    video_path = path;
    
    frame_count = cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_COUNT);
    cout << "Frame Count : " << frame_count << endl;
    FPS = cvGetCaptureProperty(capture,CV_CAP_PROP_FPS);
    cout << "FPS :"<< FPS <<endl;
    
    ////Grabbing frames from video
    cout << "Grabbing frames from the video\n";
    if(end < 0 || end > frame_count)
        end = frame_count;
    if(start < 0 || start > frame_count)
        start = 0;
    
    frame_count = end - start + 1;
    frame_start = start;
    
    cvSetCaptureProperty( capture, CV_CAP_PROP_POS_FRAMES , start );
    
    for(int i = 0 ; i < frame_count ; i ++)
    {   
        
        frame  = cvQueryFrame(capture);
        
        cout << i + 1<< "/" << frame_count << endl;
        
        if(frame)
        {   
            num_frames++;
            frameList.push_back(temp);
            
            //Ground truth detect
            myObjDetector->ground_truth_detect(this, i, frame);

            //Real detect
            //myObjDetector->detect(this, i, frame);
            
            
            //cout << frameList.size() << endl;
            if(show_detection_result){
                playImage_with_detected_results(pause_when_detected, frame);   
            }           

            /*
            myTemporalPyramid->loadFrames_realtime(this);
            if((i%FPS) == 0){
                ObjectDetector_Evaluation(i,myTemporalPyramid);  
            }
            */

            /*
            //Building pyramid
            if(frameList.size()/FPS >= MIN_NUM_ACT_SEQUENCE){
                myTemporalPyramid->loadFrames_realtime(this);
                //cout << "num of levels in pyramids:" << myTemporalPyramid->num_of_levels <<endl;
                //cout << (int)log2(frameList.size()/FPS) <<endl; 
                myTemporalPyramid->buildPyramid((int)log2(frameList.size()/FPS));

                //Showing pyramids
                for(int l = 0 ; l < myTemporalPyramid->num_of_levels ; l++)
                    myTemporalPyramid->showPyramid(l);

                //Activity Detection
                myTemporalPyramid->activity_detect(this, MIN_NUM_ACT_SEQUENCE);
            }
            */
        }
        else
        {
            break;
        }

        if(cvWaitKey(10) >= 0)
            break;
    }
    
    cvReleaseCapture(&capture);    
    cvDestroyWindow("Obj Detection Result");
    delete myObjDetector;
    delete myTemporalPyramid;
    
    return true;
}

bool FrameModel::playVideo(){
    
    if (frameList.size() < 1) {
        return false;
    }
    
    cvNamedWindow("AVI player",0);
    
    ////Show frames grabed
    cout << "Playing the selected frames\n";
    for(unsigned int i = 0; i < frameList.size(); ++i){
        
        //cvShowImage("AVI player",frameList[i].frame);
        
        if(cvWaitKey(10) >= 0)
            break;
    }
    
    cvWaitKey();
    cvDestroyWindow("AVI player");
    
    return true;
}

bool FrameModel::showFeature(int index){
    
    if (frameList.size() < index + 1) {
        return false;
    }
    
    /*
     Show frames
     */
    cout << "frame:" << index << " fetures:\n";
    for(unsigned int i = 0; i < frameList[index].feature.size(); ++i){
        cout << frameList[index].feature[i]<< " ";
    }
    
    cout << endl;
    
    return true;
}

bool FrameModel::playImage_with_detected_results(bool pause_when_detected, IplImage *tempFrame){
    

    for(int feature_index = 0 ; feature_index < num_features ; feature_index ++)
    {   
        //cout << "feature_index : " << feature_index << " " << feature_name[feature_index] << endl;
        //cout << "num of detections : " << frameList.back().result_list[feature_index].size() << endl;
        //cout << "feature_index" << feature_index <<endl;   
            for (int box = 0 ; box < frameList.back().result_list[feature_index].size() ; box++){     
                //cout << "drawing bboxes : " << box << endl;
                CvPoint point1, point2;  
                
                point1.x = frameList.back().result_list[feature_index][box].x;  
                point2.x = frameList.back().result_list[feature_index][box].x + frameList.back().result_list[feature_index][box].width;  
                point1.y = frameList.back().result_list[feature_index][box].y;  
                point2.y = frameList.back().result_list[feature_index][box].y + frameList.back().result_list[feature_index][box].height;  
                //cout << "x1:" << point1.x << " x2:" << point2.x << " y1:"<< point1.y << " y2:" <<point2.y <<endl;
                cvRectangle(tempFrame, point1, point2, CV_RGB(0,255,0), 3, 8, 0);
                //cout << "a" << endl;
                /*
                 Put the object name on the box
                 */
                CvFont font;
                cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);
                cvPutText(tempFrame, feature_name[feature_index].c_str(), cvPoint(point2.x+10, point2.y), &font, cvScalar(255, 255, 255, 0));
                //cout << "b" << endl;
            }  
            
            /*
             scale down the image since it's 720x1280 sometimes exceeds the monitor size
             */
            IplImage *dst = 0;       
            float scale = 0.5; 
            CvSize dst_cvsize;      
            dst_cvsize.width = tempFrame->width * scale;
            dst_cvsize.height = tempFrame->height * scale;
            dst = cvCreateImage( dst_cvsize, tempFrame->depth, tempFrame->nChannels);
            cvResize(tempFrame, dst, CV_INTER_LINEAR);
            
            
            /*
             Show the result in the window.  
             */
            cvShowImage("Obj Detection Result", dst);
            if(pause_when_detected && frameList.back().result_list[feature_index].size() > 0)
                cvWaitKey(0);
        
            cvReleaseImage(&dst);
            
    }

    return true;
}


bool FrameModel::playVideo_with_detected_results(bool pause_when_detected){
    
    if (frameList.size() < 1) {
        return false;
    }
    
    CvCapture *capture;
    IplImage *tempFrame;
    
    ////Laoding video file
    cout << "Laoding video file: " << video_path << endl;
    capture = cvCaptureFromAVI(video_path.c_str());
    
    cvSetCaptureProperty( capture, CV_CAP_PROP_POS_FRAMES , frame_start );
    
    cvNamedWindow("Obj Detection Result", CV_WINDOW_AUTOSIZE);
    cvMoveWindow("Obj Detection Result", 100, 0);
    
    ////Show frames grabed
    cout << "Playing the selected frames\n";
    for(unsigned int i = 0; i < frameList.size(); ++i){
        
        cout << "frameList index : " << i << endl;
        tempFrame  = cvQueryFrame(capture);
        
        for(int feature_index = 0 ; feature_index < num_features ; feature_index ++)
        {   
            cout << "feature_index : " << feature_index << " " << feature_name[feature_index] << endl;
            cout << "num of detections : " << frameList[i].result_list[feature_index].size() << endl;
                
                for (int box = 0 ; box < frameList[i].result_list[feature_index].size() ; box++){     
                    cout << "drawing bboxes : " << box << endl;
                    CvPoint point1, point2;  
                    
                    point1.x = frameList[i].result_list[feature_index][box].x;  
                    point2.x = frameList[i].result_list[feature_index][box].x + frameList[i].result_list[feature_index][box].width;  
                    point1.y = frameList[i].result_list[feature_index][box].y;  
                    point2.y = frameList[i].result_list[feature_index][box].y + frameList[i].result_list[feature_index][box].height;  
                    //cout << "x1:" << point1.x << " x2:" << point2.x << " y1:"<< point1.y << " y2:" <<point2.y <<endl;
                    cvRectangle(tempFrame, point1, point2, CV_RGB(0,255,0), 3, 8, 0);
                    
                    /*
                     Put the object name on the box
                     */
                    CvFont font;
                    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);
                    cvPutText(tempFrame, feature_name[feature_index].c_str(), cvPoint(point2.x+10, point2.y), &font, cvScalar(255, 255, 255, 0));
                }  
                
                /*
                 scale down the image since it's 720x1280 sometimes exceeds the monitor size
                 */
                IplImage *dst = 0;       
                float scale = 0.5; 
                CvSize dst_cvsize;      
                dst_cvsize.width = tempFrame->width * scale;
                dst_cvsize.height = tempFrame->height * scale;
                dst = cvCreateImage( dst_cvsize, tempFrame->depth, tempFrame->nChannels);
                cvResize(tempFrame, dst, CV_INTER_LINEAR);
                
                
                /*
                 Show the result in the window.  
                 */
                cvShowImage("Obj Detection Result", dst);
                if(pause_when_detected && frameList[i].result_list[feature_index].size() > 0)
                    cvWaitKey(0);
            
                cvReleaseImage(&dst);
                
            }
        
        if(cvWaitKey(10) >= 0)
            break;
    }
    
    //cvReleaseImage( &tempFrame );
    cvDestroyWindow("Obj Detection Result"); 
    
    cvReleaseCapture(&capture);
    
    return 0;
}

FrameModel::FrameModel(){
    num_frames = 0;
}

FrameModel::~FrameModel(){
}
