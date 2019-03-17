
# Copyright 2018 The TensorFlow Authors All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ==============================================================================

""" Offline data generation for the Cityscapes dataset."""

import os
from absl import app
from absl import flags
from absl import logging
import numpy as np
import cv2
import os, glob

import alignment
from alignment import compute_overlap
from alignment import align


SKIP = 2
WIDTH = 416
HEIGHT = 128
SUB_FOLDER = 'train'
INPUT_DIR = './ss2_data'
OUTPUT_DIR = './processed_ss2_data'

def crop(img, segimg, fx, fy, cx, cy):
    # Perform center cropping, preserving 50% vertically.
    middle_perc = 0.50
    left = 1 - middle_perc
    half = left / 2
    a = img[int(img.shape[0]*(half)):int(img.shape[0]*(1-half)), :]
    aseg = segimg[int(segimg.shape[0]*(half)):int(segimg.shape[0]*(1-half)), :]
    cy /= (1 / middle_perc)

    # Resize to match target height while preserving aspect ratio.
    wdt = int((float(HEIGHT)*a.shape[1]/a.shape[0]))
    x_scaling = float(wdt)/a.shape[1]
    y_scaling = float(HEIGHT)/a.shape[0]
    b = cv2.resize(a, (wdt, HEIGHT))
    bseg = cv2.resize(aseg, (wdt, HEIGHT))

    # Adjust intrinsics.
    fx*=x_scaling
    fy*=y_scaling
    cx*=x_scaling
    cy*=y_scaling

    # Perform center cropping horizontally.
    remain = b.shape[1] - WIDTH
    cx /= (float(b.shape[1]) / WIDTH)
    c = b[:, int(remain/2):b.shape[1]-int(remain/2)]
    cseg = bseg[:, int(remain/2):b.shape[1]-int(remain/2)]

    return c, cseg, fx, fy, cx, cy


def run_all():
  print('Processing directory', INPUT_DIR)
  seq_nr = -1
  for location in os.listdir(INPUT_DIR):
    seq_nr += 1
    fx = fy = 2.9
    cx = 960
    cy = 540
    if os.path.isdir(os.path.join(INPUT_DIR, location)):
        location_name = os.path.join(INPUT_DIR, location)
        print('Processing location', location_name)
        files = sorted(glob.glob(location_name + '/*.jpg'))
        files = [file for file in files if '-seg.jpg' not in file]
        # Break down into sequences
        sequences = {}
        seq_nr = 0
        last_seq = ''
        last_imgnr = -1

        for i in range(len(files)):
            # seq = os.path.basename(files[i]).split('_')[1]
            # nr = int(seq)
            # nr = int(os.path.basename(files[i]).split('_')[2])
            # if seq!=last_seq or last_imgnr+1!=nr:
            #    seq_nr+=1
            # last_imgnr = nr
            # last_seq = seq
            if not seq_nr in sequences:
                sequences[seq_nr] = []
            sequences[seq_nr].append(files[i])

        for (k,v) in sequences.items():
            print('Processing sequence', k, 'with', len(v), 'elements...')
            output_dir = OUTPUT_DIR + '/' + location
            if not os.path.isdir(output_dir):
                os.mkdir(output_dir)
            files = sorted(v)
            triplet = []
            seg_triplet = []
            ct = 1

            # TODO: Check these


            for j in range(0, len(files), SKIP):
                img = cv2.imread(files[j])
                # segimg = cv2.imread(files[j])
                ORIGINAL_HEIGHT, ORIGINAL_WIDTH, _ = img.shape

                # zoom_x = WIDTH / ORIGINAL_WIDTH
                # zoom_y = HEIGHT / ORIGINAL_HEIGHT

                # fx *= zoom_x
                # cx *= zoom_x
                # fy *= zoom_y
                # cy *= zoom_y

                img = cv2.resize(img, (WIDTH, HEIGHT))

                # smallimg, segimg, fx_this, fy_this, cx_this, cy_this = crop(img, segimg, fx, fy, cx, cy)
                triplet.append(img)
                # seg_triplet.append(segimg)
                if len(triplet)==3:
                    cmb = np.hstack(triplet)
                    # align1, align2, align3 = align(triplet[0], triplet[1], triplet[2])
                    # align1, align2, align3 = triplet
                    # cmb_seg = np.hstack([align1, align2, align3])
                    cv2.imwrite(os.path.join(output_dir, str(ct).zfill(10) + '.jpg'), cmb)
                    # cv2.imwrite(os.path.join(output_dir, str(ct).zfill(10) + '-fseg.jpg'), cmb_seg)
                    f = open(os.path.join(output_dir, str(ct).zfill(10) + '_cam.txt'), 'w')
                    f.write(str(fx) + ',0.0,' + str(cx) + ',0.0,' + str(fy) + ',' + str(cy) + ',0.0,0.0,1.0')
                    f.close()
                    del triplet[0]
                    # del seg_triplet[0]
                    ct+=1


    # Create file list for training. Be careful as it collects and includes all files recursively.
    fn = open(OUTPUT_DIR + '/' + SUB_FOLDER + '.txt', 'w')
    out_dir = OUTPUT_DIR
    if not os.path.exists(out_dir):
        os.mkdir(out_dir)
    for f in sorted(glob.glob(OUTPUT_DIR + '/*/*.jpg')):
        if '-seg.jpg' in f or '-fseg.jpg' in f:
            continue
        folder_name = f.split('/')[-2]
        img_name = f.split('/')[-1].replace('.jpg', '')
        fn.write(folder_name + ' ' + img_name + '\n')
    fn.close()


def main(_):
  run_all()


if __name__ == '__main__':
  app.run(main)
