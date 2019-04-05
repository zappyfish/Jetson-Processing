import sys
import os
import numpy as np
import cv2


def rotate_and_save(flight_path):
    imgs = os.listdir(os.path.join(flight_path, "images"))
    if not os.path.exists(os.path.join(flight_path, "rotated")):
        os.mkdir(os.path.join(flight_path, "rotated"))
    for img_name in imgs:
        img = cv2.imread(os.path.join(flight_path, "images", img_name))
        rot = np.rot90(img)
        save_name = os.path.join(flight_path, "rotated", img_name)
        cv2.imwrite(save_name, rot)


if __name__ == '__main__':
    script, path = sys.argv
    rotate_and_save("pre-processed-data/" + path)
