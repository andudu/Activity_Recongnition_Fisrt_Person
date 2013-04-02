FP_ADL_new
==========

## Components

- main
  - 程式入口
- haar_cascade
  - 輸入事先訓練好的單一個cascade classifiers(EX: cup.xml), 包裝成操作點
- frames
  - 主要物件, 負責讀取split frames, 並online呼叫以下兩種物件來進行辨識 
- temporalPyramid
  - 輸入frame物件, 建立temporal pyramid  
- objectDetector
  - 容納所有haar_cascade物件, 包裝給frame物件使用
