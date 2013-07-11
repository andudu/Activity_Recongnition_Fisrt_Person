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


## Branches

- master
  - 目前無用(未整理)
- demo
  - 讀取CVPR12 dataset的影片split frames與object annotation, 進行辨識
- dpm
  - 讀取CVPR12 dataset的影片split frames與object model, 進行辨識
- my_data_ground_truth
  - 讀取自製dataset的影片split frames與object annotation, 進行辨識
- my_data_real_object
  - 讀取自製dataset的影片split frames與object model, 進行辨識

