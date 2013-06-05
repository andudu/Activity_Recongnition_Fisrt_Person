echo "run by batch"

frame_numbers=(
0
10831
12481
14551
12331
13021
)

thres="10"
the_fpn="30"
FPN="-FPN ${the_fpn}"
thres_factor="-thres_factor ${thres}"
#indicate="-indicate 2"
#show="-show"
show=""
show_pyramid="-show_pyramid"
activity_prediction="-activity_prediction"
#pause="-pause"
ground_truth_detect="-ground_truth"
crf="-crf"
start="1"

cmd="rm -r crf/result/my_data/FPN_${the_fpn}"

$cmd

cmd="mkdir -p crf/result/my_data/FPN_${the_fpn}/no_pyramid"

$cmd

cmd="mkdir -p crf/result/my_data/FPN_${the_fpn}/pyramid"

$cmd

for (( i=1; i<=5; i=i+1 ))
do

  #crf_model_path="crf/multi_stage/1_vs_all_more_segment_2/model_${i}.crf"
  #crf_model_path="crf/multi_stage/1_vs_all_more_segment/model_${i}.crf"
  #crf_model_path="crf/multi_stage/demo/model_${i}.crf"
  crf_model_path="crf/my_data/model_${i}.crf"

  length=${frame_numbers[$i]}

  if [ $i -lt 10 ];         # If $i is smaller than 10
    then
      index="0${i}"
    else
      index=$i
  fi

  #video="/Users/hmliu/Documents/CMLab/Master/ADL_code/ADLdataset/ADL_videos/split_frames_P_${index}"
  video="/Users/hmliu/Documents/CMLab/Master/ADL_code/ADLdataset/Haar_helper/my_data/videos/split_frames_P_${index}"
  #annotation="translated_with_obj_name/object_annot_P_${index}_translated_with_obj_name.txt"
  annotation="my_data_obj_annotation/P${index}.txt"

  #no_pyramid
  build_pyramid=""
  cmd="./FP_ADL.out -i ${video} -start ${start} -length ${length} -crf_model_path ${crf_model_path} -an ${annotation} ${show} ${pause} ${indicate} ${ground_truth_detect} ${crf} ${thres_factor} ${show_pyramid} ${build_pyramid} ${activity_prediction} ${FPN}"

  echo $cmd

  $cmd

  cmd="mv activity_result.txt crf/result/my_data/FPN_${the_fpn}/no_pyramid/result_${index}.txt"

  echo $cmd

  $cmd

  #with_pyramid
  build_pyramid="-build_pyramid"
  cmd="./FP_ADL.out -i ${video} -start ${start} -length ${length} -crf_model_path ${crf_model_path} -an ${annotation} ${show} ${pause} ${indicate} ${ground_truth_detect} ${crf} ${thres_factor} ${show_pyramid} ${build_pyramid} ${activity_prediction} ${FPN}"

  echo $cmd

  $cmd

  cmd="mv activity_result.txt crf/result/my_data/FPN_${the_fpn}/pyramid/result_${index}.txt"

  echo $cmd

  $cmd
done



echo "done!"