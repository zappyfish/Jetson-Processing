import tensorflow as tf
import sys

with tf.Session() as sess:
    saver = tf.train.import_meta_graph("/Users/liamkelly/vadl/models/research/struct2depth/struct2depth_model_kitti/model-199160.meta")
    #saver.restore(sess, "/Users/liamkelly/vadl/models/research/struct2depth/struct2depth_model_kitti/model-199160.ckpt")
    saver.restore(sess, "/Users/liamkelly/vadl/models/research/struct2depth/struct2depth_model_kitti/model-199160")