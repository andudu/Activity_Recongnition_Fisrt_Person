echo "run by shell"

index="03"
video="/Users/hmliu/Documents/CMLab/Master/ADL_code/ADLdataset/ADL_videos/P_${index}.MP4"
annotation="translated_with_obj_name/object_annot_P_${index}_translated_with_obj_name.txt"
start="0"
end="30"
indicate="1"
show="-show"
pause=""

cmd="./FP_ADL.out -i ${video} -start ${start} -end ${end} -an ${annotation} ${show} ${pause} -indicate ${indicate}"

echo $cmd

$cmd

echo "shell done"