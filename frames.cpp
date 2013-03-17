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

int FrameModel::getFPS(){

    return FPS;
}

bool FrameModel::loadVideo_realtime(string path, bool pause_when_detected ,int start, int end){
    
    ObjectDetector* myObjDetector = new ObjectDetector;
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
            //temp.frame = cvCloneImage(frame);
            myObjDetector->detect(this, i, frame);
            //cout << frameList.size() << endl;
            playImage_with_detected_results(pause_when_detected, frame);

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

bool FrameModel::loadVideo(string path, int start, int end){
    
    CvCapture *capture;
    IplImage *frame;
    ObjectDetector* myObjDetector = new ObjectDetector;
    
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
    
    frameNode temp;
    
    for(int i = 0 ; i < frame_count ; i ++)
    {   
        
        frame  = cvQueryFrame(capture);
        
        cout << i + 1<< "/" << frame_count << endl;
        
        if(frame)
        {   
            
            frameList.push_back(temp);
            //temp.frame = cvCloneImage(frame);
            myObjDetector->detect(this, i, frame);
            
        }
        else
        {
            break;
        }
    }
    
    cvReleaseCapture(&capture);
    
    delete myObjDetector;
    
    return true;
}

bool FrameModel::loadVideo(string path){
    
    CvCapture *capture;
    IplImage *frame;
    int start;
    int end;
    ObjectDetector* myObjDetector = new ObjectDetector;
    
    ////Laoding video file
    cout << "Laoding video file\n";
    capture = cvCaptureFromAVI(path.c_str());
    video_path = path;
    
    frame_count = cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_COUNT);
    cout << "Frame Count : " << frame_count << endl;
    FPS = cvGetCaptureProperty(capture,CV_CAP_PROP_FPS);
    cout << "FPS :"<< FPS <<endl;
    
    cout << "start:";
    cin >> start;
    cout << "end:";
    cin >> end;
    
    ////Grabbing frames from video
    cout << "Grabbing frames from the video\n";
    if(end < 0 || end > frame_count)
        end = frame_count;
    if(start < 0 || start > frame_count)
        start = 0;
    
    frame_count = end - start + 1;
    frame_start = start;
    
    cvSetCaptureProperty( capture, CV_CAP_PROP_POS_FRAMES , start );
    
    frameNode temp;

    for(int i = 0 ; i < frame_count ; i ++)
    {   
        
        frame  = cvQueryFrame(capture);
        
        cout << i + 1<< "/" << frame_count << endl;
        
        if(frame)
        {   
            frameList.push_back(temp);
            //temp.frame = cvCloneImage(frame);
            myObjDetector->detect(this, i, frame);
            
        }
        else
        {
            break;
        }
    }
    
    cvReleaseCapture(&capture);
    
    delete myObjDetector;

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
            
            for (int box = 0 ; box < frameList.back().result_list[feature_index].size() ; box++){     
                //cout << "drawing bboxes : " << box << endl;
                CvPoint point1, point2;  
                
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
