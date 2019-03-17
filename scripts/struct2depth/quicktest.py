import os
import cv2

data_dir = '/Users/liamkelly/vadl/models/research/struct2depth/lab/data2'

for f in os.listdir(data_dir):
    full = os.path.join(data_dir, f)
    if '.jpg' in full:
        img = cv2.imread(full)
        print(img.shape)