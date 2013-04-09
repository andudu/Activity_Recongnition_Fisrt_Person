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

for (( i=1; i<=9; i=i+1 ))
do

  index=$i

  video="/Users/hmliu/Documents/CMLab/Master/ADL_code/ADLdataset/ADL_videos/split_frames_P_0${index}"
  annotation="translated_with_obj_name/object_annot_P_0${index}_translated_with_obj_name.txt"
  #crf_model_path="crf/multi_stage/model_${index}.crf"
  crf_model_path="crf/multi_stage/1_vs_all_more_segment/model_${index}.crf"

  start="1"

  length=${frame_numbers[$index]}

  echo $length
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

  cmd="mv activity_result.txt crf/result/more_segment/result_${index}.txt"

  echo $cmd

  $cmd
done


for (( i=10; i<=20; i=i+1 ))
do

  index=$i

  video="/Users/hmliu/Documents/CMLab/Master/ADL_code/ADLdataset/ADL_videos/split_frames_P_${index}"
  annotation="translated_with_obj_name/object_annot_P_${index}_translated_with_obj_name.txt"
  #crf_model_path="crf/multi_stage/model_${index}.crf"
  crf_model_path="crf/multi_stage/1_vs_all_more_segment/model_${index}.crf"

  start="1"

  length=${frame_numbers[$index]}

  echo $length
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

  cmd="mv activity_result.txt crf/result/more_segment/result_${index}.txt"

  echo $cmd

  $cmd
done



echo "done!"