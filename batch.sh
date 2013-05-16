echo "run by batch"

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


start="1"
#indicate="-indicate 2"
show="-show"
#show_pyramid="-show_pyramid"
build_pyramid="-build_pyramid"
activity_prediction="-activity_prediction"
#pause="-pause"
ground_truth_detect="-ground_truth"
crf="-crf"
FPN="-FPN 300"
thres_factor="-thres_factor 10"
dpm_thres="-dpm_thres -0.7"
result_path="crf/result/0516/pyramid/result_${index}.txt"


for (( i=1; i<=20; i=i+1 ))
do


  if [ $i -lt 10 ];         # If $i is smaller than 10
    then
        index="0${i}"
    else
        index=$i
  fi

  video="/Users/hmliu/Documents/CMLab/Master/ADL_code/ADLdataset/ADL_videos/split_frames_P_0${index}"
  #annotation="translated_with_obj_name/object_annot_P_0${index}_translated_with_obj_name.txt"
  annotation="dpm_with_obj_name/P_${index}.txt"
  crf_model_path="crf/multi_stage/1_vs_all_more_segment_2/model_${index}.crf"
  length=${frame_numbers[$i]}
  
  cmd="./FP_ADL.out -i ${video} -start ${start} -length ${length} -crf_model_path ${crf_model_path} -an ${annotation} ${show} ${pause} ${indicate} ${ground_truth_detect} ${crf} ${thres_factor} ${show_pyramid} ${build_pyramid} ${activity_prediction} ${FPN} ${dpm_thres}"

  echo $cmd

  $cmd

  cmd="mv activity_result.txt ${result_path}"

  echo $cmd

  $cmd
done


echo "done!"