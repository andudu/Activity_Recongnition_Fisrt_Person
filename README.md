Activity_Recongnition_Fisrt_Person
==========

## Components

- main
  - 程式入口
- haar_cascade
  - 讀入事先訓練好的單一個cascade classifiers(EX: cup.xml), 包裝成操作點
- frames
  - 主要物件, 負責讀取split frames, 並online操作以下兩種物件來進行辨識
- objectDetector
  - 容納所有haar_cascade物件, 包裝給frame物件使用
- temporalPyramid
  - 輸入frame物件, 建立temporal pyramid  
- crf(folder)
  - multi_stage : 內含所有的CRF事件模型
  - result : 跑批次實驗輸出的結果
- cascade_xxxx(folder)
  - 各種cascade object detector 
- translated_with_obj_name(folder)
  - object annotation for CVPR12 dataset(Ground truth)
- dpm_with_obj_name(folder)
  - object annotation for CVPR12 dataset(Deformable part-based model)
- my_data_obj_annotation(folder)
  - object annotation for my dataset(Ground truth)
- run.sh
  - 方便執行程式用的shell script 

## Branches

- master
  - 目前無用(未整理)
- demo
  - 讀取CVPR12 dataset的影片split frames與object annotation(ground truth), 進行辨識
- dpm
  - 讀取CVPR12 dataset的影片split frames與object annotation(Deformable part-based model), 進行辨識
- my_data_ground_truth
  - 讀取自製dataset的影片split frames與object annotation(ground truth) 進行辨識
- my_data_real_object
  - 讀取自製dataset的影片split frames與object model(LBP cascade), 進行辨識

## Reguirements
- C++ libraries
  - Boost http://www.boost.org/
  - OpenCV 2.4 以上
- CVPR12 dataset
  - http://deepthought.ics.uci.edu/ADLdataset/adl.html
  - 如欲使用demo&dpm,需下載ADL_videos,將各影片拆解成圖片後各自放進一資料夾,並在run.sh設定位置
- My dataset
  - not yet uploaded
  - 如欲使用my_data_ground_truth&my_data_real_obj,需下載並將各影片拆解成圖片後各自放進一資料夾,並在run.sh設定位置
- CRF++
  - http://crfpp.googlecode.com/svn/trunk/doc/index.html?source=navbar
  - CRF的部份都是採用這個工具, 在本程式裡使用system call去外部執行crf_test來做辨識, CRF model也都是用此工具做訓練
