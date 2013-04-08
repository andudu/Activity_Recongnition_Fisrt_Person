echo "run by shell"

index="11"
video="/Users/hmliu/Documents/CMLab/Master/ADL_code/ADLdataset/ADL_videos/split_frames_P_${index}"
annotation="translated_with_obj_name/object_annot_P_${index}_translated_with_obj_name.txt"
crf_model_path="crf/multi_stage/model_${index}.crf"

# Good results
# Watching TV in P02
# start="69235"

# using computer in  P02
# start="74335"

# Brushing teeth in P02
start="1"

#length="28673"

length="14774"

#indicate="-indicate 2"
show="-show"
#pyramid="-pyramid"
activity_prediction="-activity_prediction"
#pause="-pause"
ground_truth_detect="-ground_truth"
crf="-crf"
FPN="-FPN 300"
thres_factor="-thres_factor 3"

cmd="./FP_ADL.out -i ${video} -start ${start} -length ${length} -crf_model_path ${crf_model_path} -an ${annotation} ${show} ${pause} ${indicate} ${ground_truth_detect} ${crf} ${thres_factor} ${pyramid} ${activity_prediction} ${FPN}"

echo $cmd

$cmd

echo "shell done"