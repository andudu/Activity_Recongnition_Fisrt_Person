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
    
    map<string,int>::iterator it2;
    cout << "ground_truth:" <<endl;
    for(it2 = obj_name_reverse.begin() ; it2 != obj_name_reverse.end() ; it2++)
        cout<<it2->first<<" "<<it2->second<<endl;
    */

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
    VideoCapture capture(path.c_str());
    Mat grab_frame;
    IplImage frame;
    frameNode temp;
    char buffer [512];

    cvNamedWindow("Obj Detection Result", CV_WINDOW_AUTOSIZE);
    cvMoveWindow("Obj Detection Result", 50, 0);
    
    ////Laoding video file
    /*
    cout << "Laoding video file\n";
    video_path = path;
    
    frame_count = capture.get(CV_CAP_PROP_FRAME_COUNT);
    cout << "Frame Count : " << frame_count << endl;
    FPS = capture.get(CV_CAP_PROP_FPS);
    */
    FPS = 29;
    cout << "FPS :"<< FPS <<endl;
    
    ////Grabbing frames from video
    /*
    cout << "Grabbing frames from the video\n";
    if(end < 0 || end > frame_count)
        end = frame_count;
    if(start < 0 || start > frame_count)
        start = 0;
    */
    frame_count = end - start + 1;
    frame_start = start;
    
    //capture.set(CV_CAP_PROP_POS_FRAMES,start);
    //cout << start <<endl;
    //cout << "capture.Getprop:" << capture.get(CV_CAP_PROP_POS_FRAMES) << endl;

    //Testing
    /*
    cout << "\n\n\n";
    cout << "start:" << start <<endl;
    cout << "end:" << end <<endl;

    //frame  = capture.grqb();
    //capture.read(grab_frame);
    //frame = IplImage(grab_frame); 
    //walk around temporally
    char buffer [512];
    sprintf (buffer, "/Users/hmliu/Documents/CMLab/Master/ADL_code/frames_P_05/%06d.jpg",start);
    grab_frame = imread(buffer, CV_LOAD_IMAGE_COLOR);
    frame = IplImage(grab_frame);

    IplImage *dst = 0;       
    float scale = 0.5; 
    CvSize dst_cvsize;      
    dst_cvsize.width = frame.width * scale;
    dst_cvsize.height = frame.height * scale;
    dst = cvCreateImage( dst_cvsize, frame.depth, frame.nChannels);
    cvResize(&frame, dst, CV_INTER_LINEAR);
    cvShowImage("Obj Detection Result", dst);
    cvWaitKey();
    exit(1);
    */

    for(int i = 0 ; i < frame_count ; i ++)
    {   
        //cout << i + 1<< "/" << frame_count << endl;
        
        //if(capture.read(grab_frame))
        if(true)
        {   
            //frame = IplImage(grab_frame);

            //walk around Temporarily
            //sprintf (buffer, "/Users/hmliu/Documents/CMLab/Master/ADL_code/ADLdataset/ADL_videos/split_frames_%d/%06d.jpg",start+i);
            sprintf(buffer,"%s/%06d.jpg",path.c_str(),start+i);
            //cout << buffer <<endl;
            grab_frame = imread(buffer, CV_LOAD_IMAGE_COLOR);
            frame = IplImage(grab_frame);

            num_frames++;
            frameList.push_back(temp);

            //Ground truth detect
            myObjDetector->ground_truth_detect(this, i, &frame , frame_start);

            //Real detect
            //myObjDetector->detect(this, i, frame);

            //cout << frameList.size() << endl;
            
            if(show_detection_result){
                playImage_with_detected_results(pause_when_detected, &frame);   
            }           
            
            /*
            cout << "1" << endl;
            //Loading frames and put them into pyramid, level 1
            myTemporalPyramid->loadFrames_realtime(this);
            cout << "2" << endl;
            //myTemporalPyramid->print_info("num_of_levels");
            myTemporalPyramid->showPyramid(0);
            cout << "3" << endl;

            //Build the pyramid
            cout << "frameList.size():" << frameList.size() <<endl;
            cout << "FPS:" << FPS <<endl;
            cout << "level required:" << (int)log2(frameList.size()/FPS) + 1<<endl;
            myTemporalPyramid->buildPyramid((int)log2(frameList.size()/FPS) + 1);

            //Showing pyramids
            for(int l = 0 ; l < myTemporalPyramid->num_of_levels ; l++)
                myTemporalPyramid->showPyramid(l);

            /*
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
    
    //cvReleaseCapture(&capture);
    capture.release();    
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

    int detection_counter = 0;

    //cout << "num_features : " << num_features << endl;

    for(int feature_index = 0 ; feature_index < num_features ; feature_index ++)
    {   
        cout << "feature_index : " << feature_index << " " << feature_name[feature_index] << endl;
        cout << "num of detections : " << frameList.back().result_list[feature_index].size() << endl;
        //cout << "feature_index" << feature_index <<endl;   
        for (int box = 0 ; box < frameList.back().result_list[feature_index].size() ; box++){     
            //cout << "drawing bboxes : " << box << endl;
            CvPoint point1, point2;  
            cout << "box:" << box <<endl; 
            point1.x = frameList.back().result_list[feature_index][box].x;  
            point2.x = frameList.back().result_list[feature_index][box].x + frameList.back().result_list[feature_index][box].width;  
            point1.y = frameList.back().result_list[feature_index][box].y;  
            point2.y = frameList.back().result_list[feature_index][box].y + frameList.back().result_list[feature_index][box].height;  
            //cout << "x1:" << point1.x << " x2:" << point2.x << " y1:"<< point1.y << " y2:" <<point2.y <<endl;
            cvRectangle(tempFrame, point1, point2, CV_RGB(0,255,0), 3, 8, 0);
            /*
             Put the object name on the box
             */
            CvFont font;
            cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);
            cvPutText(tempFrame, feature_name[feature_index].c_str(), cvPoint(point1.x+10, point1.y+20), &font, cvScalar(255, 0, 0, 0));
            detection_counter++;
        } 
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
    if(pause_when_detected && detection_counter > 0)
        cvWaitKey(0);

    cvReleaseImage(&dst);

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
        
        int detection_counter = 0;

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
                detection_counter++;
            }                
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
        if(pause_when_detected && detection_counter > 0)
            cvWaitKey(0);
        
        cvReleaseImage(&dst);

        if(cvWaitKey(10) >= 0)
            break;        
    }
    
    //cvReleaseImage( &tempFrame );
    cvDestroyWindow("Obj Detection Result"); 
    
    cvReleaseCapture(&capture);

    return 0;
}

bool FrameModel::print_info(string info_id){
    
    if(info_id.compare("num_frames") == 0){
        cout << "num_frames:" << num_frames << endl;
        return true;
    }

    return false;
}

FrameModel::FrameModel(){
    num_frames = 0;
}

FrameModel::~FrameModel(){
}
