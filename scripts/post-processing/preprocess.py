import sys
from os import listdir, mkdir, getcwd
from os.path import isfile, join
from shutil import copyfile
import json
from pixhawk_data import FlightDataProcessor
from target_data import TargetDataProcessor
from PIL import Image
from mode_data import ModeDataProcessor, AlwaysManualMode

IMAGE_FOLDER  = "images"
PRE_PROCESSED_FOLDER = "pre-processed-data"


def preprocess(flight_data_path, pre_processed_name):
    flight_data = FlightDataProcessor(flight_data_path)
    target_data = TargetDataProcessor(flight_data_path)
    try:
        mode_data = ModeDataProcessor(flight_data_path)
    except:
        mode_data = AlwaysManualMode()
    images = [int(f.split('.')[0]) for f in listdir(join(flight_data_path, IMAGE_FOLDER)) if isfile(join(flight_data_path, IMAGE_FOLDER, f))]
    images.sort()
    image_ind = 0
    save_directory = join(getcwd(), PRE_PROCESSED_FOLDER, pre_processed_name)
    print(save_directory)
    mkdir(save_directory)
    mkdir(join(save_directory, IMAGE_FOLDER))
    processed = {}
    offset = 0
    next_target = target_data.get_next()
    while flight_data.has_next() and image_ind < len(images):
    # while image_ind < len(images):
        image_time = images[image_ind]
        while image_time <= -1: # It takes a bit of time for the logging to start, but camera capture happens in that time
            image_ind += 1
            offset += 1
            image_time = images[image_ind]
        image_name = str(images[image_ind]) + ".jpg"
        processed[image_ind - offset] = get_image_data(image_time, flight_data)
        save_name = join(save_directory, IMAGE_FOLDER, get_image_name(flight_data, str(image_ind - offset) + '.jpg'))
        processed[image_ind - offset]['path'] = save_name
        if next_target is not None and next_target['image_timestamp'] == image_time:
            processed[image_ind - offset]['target'] = True
            for key in next_target:
                processed[image_ind - offset][key] = next_target[key]
            next_target = target_data.get_next()
        else:
            processed[image_ind - offset]['target'] = False
        processed[image_ind - offset]['mode'] = mode_data.get_mode_for_time(image_time)
        copyfile(join(flight_data_path, IMAGE_FOLDER, image_name), save_name)
        image_ind += 1

    with open(join(save_directory, 'data.json'), 'w') as fp:
        json.dump(processed, fp)


def get_image_name(flight_data, original_name):
    diff = flight_data.max_digits - (len(original_name) - 4)
    for i in range(diff):
        original_name = '0' + original_name
    return original_name


def get_image_data(image_time, flight_data):
    if flight_data.has_next():
        closest_data = flight_data.get_next()
    else:
        closest_data = flight_data.get_last()
    while flight_data.has_next() and abs(image_time - flight_data.peek_time()) <= abs(image_time - closest_data['time']):
        closest_data = flight_data.get_next()
    return closest_data

if __name__ == '__main__':
    script, path, name = sys.argv
    preprocess(path, name)