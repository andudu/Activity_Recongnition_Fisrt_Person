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

#Fold_1
fold_1=(10 14 16 20)

for (( i=0; i<=3; i=i+1 ))
do

  index=${fold_1[$i]}

  video="/Users/hmliu/Documents/CMLab/Master/ADL_code/ADLdataset/ADL_videos/split_frames_P_${index}"
  annotation="translated_with_obj_name/object_annot_P_${index}_translated_with_obj_name.txt"
  crf_model_path="crf/multi_stage/5_fold/model_fold_1.crf"

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

  cmd="mv activity_result.txt crf/result/fold_1/result_${index}.txt"

  echo $cmd

  $cmd
done

#####################################
#Fold_2
fold_2=(8 13 18 19 )

for (( i=0; i<=3; i=i+1 ))
do

  index=${fold_2[$i]}

  video="/Users/hmliu/Documents/CMLab/Master/ADL_code/ADLdataset/ADL_videos/split_frames_P_${index}"
  annotation="translated_with_obj_name/object_annot_P_${index}_translated_with_obj_name.txt"
  crf_model_path="crf/multi_stage/5_fold/model_fold_2.crf"

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

  cmd="mv activity_result.txt crf/result/fold_2/result_${index}.txt"

  echo $cmd

  $cmd
done

#####################################
#Fold_3
fold_3=(1 6 11 17 )

for (( i=0; i<=3; i=i+1 ))
do

  index=${fold_3[$i]}

  video="/Users/hmliu/Documents/CMLab/Master/ADL_code/ADLdataset/ADL_videos/split_frames_P_${index}"
  annotation="translated_with_obj_name/object_annot_P_${index}_translated_with_obj_name.txt"
  crf_model_path="crf/multi_stage/5_fold/model_fold_3.crf"

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

  cmd="mv activity_result.txt crf/result/fold_3/result_${index}.txt"

  echo $cmd

  $cmd
done

#####################################
#Fold_4
fold_4=(4 5 7 15 )

for (( i=0; i<=3; i=i+1 ))
do

  index=${fold_4[$i]}

  video="/Users/hmliu/Documents/CMLab/Master/ADL_code/ADLdataset/ADL_videos/split_frames_P_${index}"
  annotation="translated_with_obj_name/object_annot_P_${index}_translated_with_obj_name.txt"
  crf_model_path="crf/multi_stage/5_fold/model_fold_4.crf"

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

  cmd="mv activity_result.txt crf/result/fold_4/result_${index}.txt"

  echo $cmd

  $cmd
done

#####################################
#Fold_5
fold_5=(2 3 9 12 )

for (( i=0; i<=3; i=i+1 ))
do

  index=${fold_5[$i]}

  video="/Users/hmliu/Documents/CMLab/Master/ADL_code/ADLdataset/ADL_videos/split_frames_P_${index}"
  annotation="translated_with_obj_name/object_annot_P_${index}_translated_with_obj_name.txt"
  crf_model_path="crf/multi_stage/5_fold/model_fold_5.crf"

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

  cmd="mv activity_result.txt crf/result/fold_5/result_${index}.txt"

  echo $cmd

  $cmd
done

echo "done!"