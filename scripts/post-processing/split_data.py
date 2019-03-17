import sys
import cv2
from os.path import join, isdir, exists
from os import mkdir
import json

PRE_PROCESSED_FOLDER = "pre-processed-data"


def load_flight_data(flight_name):
    flight = join(PRE_PROCESSED_FOLDER, flight_name)
    if not isdir(flight):
        print("Flight directory %s Could Not Be Found" % flight)
        return None
    else:
        print("Found flight directory %s. Starting playback." % flight)
        return json.load(open(join(flight, 'data.json')))

def create_split(path):
    flight_data = load_flight_data(path)
    dir_cnt = 0
    for i in range(len(flight_data)):
        data_pt = flight_data[str(i)]
        img_path = data_pt['path']
        splt = img_path.split('/')
        img_name = splt[len(splt) - 1]
        img = cv2.imread(img_path)
        cv2.imshow('frame', img)
        k = cv2.waitKey(0)
        if k == ord('n'):
            dir_cnt += 1
        if k != ord('s'):
            save_to_dir(path, dir_cnt, img, img_name)

def save_to_dir(path, dir_cnt, img, img_name):
    dir_name = path + '_' + str(dir_cnt)
    if not exists(dir_name):
        mkdir(dir_name)
    cv2.imwrite(join(dir_name, img_name), img)


if __name__ == '__main__':
    script, path = sys.argv
    create_split(path)