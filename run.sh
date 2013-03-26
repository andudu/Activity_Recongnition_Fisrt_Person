echo "run by shell"

index="02"
#video="/Users/hmliu/Documents/CMLab/Master/ADL_code/ADLdataset/ADL_videos/P_${index}.MP4"
video="/Users/hmliu/Documents/CMLab/Master/ADL_code/ADLdataset/ADL_videos/split_frames_P_${index}"
annotation="translated_with_obj_name/object_annot_P_${index}_translated_with_obj_name.txt"
start="3000"
length="120"
indicate="1"
#show=""
show="-show"
pause=""
#pause="-pause"

cmd="./FP_ADL.out -i ${video} -start ${start} -length ${length} -an ${annotation} ${show} ${pause} -indicate ${indicate}"

echo $cmd

$cmd

echo "shell done"