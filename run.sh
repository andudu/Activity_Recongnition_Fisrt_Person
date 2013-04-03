echo "run by shell"

index="02"
video="/Users/hmliu/Documents/CMLab/Master/ADL_code/ADLdataset/ADL_videos/split_frames_P_${index}"
annotation="translated_with_obj_name/object_annot_P_${index}_translated_with_obj_name.txt"
start="69235"
length="120"
indicate="-indicate 2"
show="-show"
#pause="-pause"
ground_truth_detect="-ground_truth"
crf="-crf"
thres_factor="-thres_factor 5"

cmd="./FP_ADL.out -i ${video} -start ${start} -length ${length} -an ${annotation} ${show} ${pause} ${indicate} ${ground_truth_detect} ${crf} ${thres_factor}"

echo $cmd

$cmd

echo "shell done"