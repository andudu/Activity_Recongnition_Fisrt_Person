echo "run by shell"

index="02"
video="/Users/hmliu/Documents/CMLab/Master/ADL_code/ADLdataset/ADL_videos/split_frames_P_${index}"
annotation="translated_with_obj_name/object_annot_P_${index}_translated_with_obj_name.txt"

# Good results
# Watching TV in P02
# start="69235"

# using computer in  P02
# start="74335"

# Brushing teeth in P02
start="500"

#start="29130"
length="300"
#indicate="-indicate 2"
show="-show"
#pyramid="-pyramid"
activity_prediction="-activity_prediction"
#pause="-pause"
ground_truth_detect="-ground_truth"
crf="-crf"
FPN="-FPN 30"
thres_factor="-thres_factor 2"

cmd="./FP_ADL.out -i ${video} -start ${start} -length ${length} -an ${annotation} ${show} ${pause} ${indicate} ${ground_truth_detect} ${crf} ${thres_factor} ${pyramid} ${activity_prediction} ${FPN}"

echo $cmd

$cmd

echo "shell done"