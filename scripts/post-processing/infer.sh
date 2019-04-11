
norm="/Users/liamkelly/vadl/nvidia/scripts/post-processing/pre-processed-data/$1/images"

rot="/Users/liamkelly/vadl/nvidia/scripts/post-processing/pre-processed-data/$1/rotated"

model_checkpoint="/Users/liamkelly/vadl/training/struct2depth/struct2depth_model_kitti/model-199160"

outnorm="/Users/liamkelly/vadl/nvidia/scripts/post-processing/pre-processed-data/$1/s2d/norm"

outrot="/Users/liamkelly/vadl/nvidia/scripts/post-processing/pre-processed-data/$1/s2d/rot"

python3 /Users/liamkelly/vadl/training/struct2depth/inference.py \
    --logtostderr \
    --file_extension jpg \
    --depth \
    --egomotion true \
    --input_dir $norm \
    --output_dir $outnorm \
    --model_ckpt $model_checkpoint


python3 /Users/liamkelly/vadl/training/struct2depth/inference.py \
    --logtostderr \
    --file_extension jpg \
    --depth \
    --egomotion true \
    --input_dir $rot \
    --output_dir $outrot \
    --model_ckpt $model_checkpoint