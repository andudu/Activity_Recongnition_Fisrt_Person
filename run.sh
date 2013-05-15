echo "run by shell"

index="20"
video="/Users/hmliu/Documents/CMLab/Master/ADL_code/ADLdataset/ADL_videos/split_frames_P_${index}"
#annotation="translated_with_obj_name/object_annot_P_${index}_translated_with_obj_name.txt"
annotation="dpm_with_obj_name/P_${index}.txt"
crf_model_path="crf/multi_stage/1_vs_all_more_segment_2/model_${index}.crf"

start="1"
length="600"

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
# start="500"git 
# length="900"
# making tea stage 2 in P13 
# start="21810"
# length="900"

# A temporal pyramid demo P20 washing dishes
# start="13000"
# length="800"

# making cold food snack in P10
# start="1000"
# length="2000"

# A 3 activities demo in P13
start="42800"
length="1600"

#indicate="-indicate 2"
show="-show"
show_pyramid="-show_pyramid"
build_pyramid="-build_pyramid"
activity_prediction="-activity_prediction"
#pause="-pause"
ground_truth_detect="-ground_truth"
crf="-crf"
FPN="-FPN 300"
thres_factor="-thres_factor 10"
dpm_thres="-dpm_thres -0.65"

cmd="./FP_ADL.out -i ${video} -start ${start} -length ${length} -crf_model_path ${crf_model_path} -an ${annotation} ${show} ${pause} ${indicate} ${ground_truth_detect} ${crf} ${thres_factor} ${show_pyramid} ${build_pyramid} ${activity_prediction} ${FPN} ${dpm_thres}"

echo $cmd

$cmd

echo "shell done"