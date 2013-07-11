echo "run by shell"

#欲辨識影片代號
i="13"

#選擇CRF model(因為cross-validation,故每個video只對應到一個CRF model)
crf_model_path="crf/multi_stage/demo/model_${i}.crf"

if [ $i -lt 10 ];         # If $i is smaller than 10
  then
    index="0${i}"
  else
    index=$i
fi

#影片拆解畫格的位置
video="/Users/hmliu/Documents/CMLab/Master/ADL_code/ADLdataset/ADL_videos/split_frames_P_${index}"

#Object annotation的位置
annotation="translated_with_obj_name/object_annot_P_${index}_translated_with_obj_name.txt"

#各影片長度(再run.sh裡沒有用到僅供參考)
frame_numbers=(
0
53804
85772
71090
47327
44228
46484
79277
47738
38630
28673
14774
25316
53021
45920
47525
25196
26546
34475
113885
48254
)

# 設定影片起點終點
start="42800"
length="1000"

# 一些Good Cases
# Watching TV in P02
# start="69235"
# length="1000"

# using computer in  P02 (simple and boring)
# start="74335"
# length="600"

# Brushing teeth in P02
# A temporal pyramid demo 
# start="500"
# length="1200"

# making tea stage 1 in P13 
# start="500"
# length="1200"
# making tea stage 2 in P13 
# start="21810"
# length="900"

# A temporal pyramid demo P20 washing dishes
# start="13000"
# length="800"

# making cold food snack in P10
#start="1000"
#length="2000"

# A 3 activities demo in P13
# start="42800"
# length="1600"

# 操作選項,註解掉即為不啟用
# 是否展示影片與偵物件測結果
show="-show"
# 是否在console印出temporal pyramid
show_pyramid="-show_pyramid"
# 是否建立temporal pyramid.不啟動的話pyramid只會有level 0,第一層
build_pyramid="-build_pyramid"
# 當偵測到物件是否暫停影片畫面
# pause="-pause"
# 是否使用ground truth object annotation(就是本shell第20行指定的檔案)
ground_truth_detect="-ground_truth"
# 是否呼叫CRF++進行行為辨識
crf="-crf"
# 是否印出行為辨識結果
activity_prediction="-activity_prediction"
# Frame per segment in level 0
FPN="-FPN 300"
# 當物件存在一segemt中的次數>FPN/thres_factor, 視為本segment的feature
thres_factor="-thres_factor 10"

cmd="./FP_ADL.out -i ${video} -start ${start} -length ${length} -crf_model_path ${crf_model_path} -an ${annotation} ${show} ${pause} ${ground_truth_detect} ${crf} ${thres_factor} ${show_pyramid} ${build_pyramid} ${activity_prediction} ${FPN}"

echo $cmd

$cmd

echo "shell done"
