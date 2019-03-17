import cv2
from os.path import join, isdir
from os import getcwd
import json
import sys
from net_results import NetResultsParser
import math
from math import fabs
import numpy as np

PRE_PROCESSED_FOLDER = "pre-processed-data"


def create_frame(image, x, y, z, roll, pitch, yaw, time, mode,target, ego=None, initial=None):
    font = cv2.FONT_HERSHEY_SIMPLEX
    h, w, d = image.shape
    w = w / 2
    h = h / 2

    y = 50
    text = "TIME: %d, height: %d ft" % (time, z * 0.0328084 - 5)
    cv2.putText(image, text, (20, y), font, 0.6, (0, 0, 255), 3, cv2.LINE_AA)
    y+=50
    text = "roll: % f, pitch: % f, yaw: % f" % (roll, pitch, yaw)
    cv2.putText(image, text, (20, y), font, 0.6, (0, 0, 255), 3, cv2.LINE_AA)
    y+=50
    cv2.putText(image, mode, (20, y), font, 2, (255, 255, 255), 3, cv2.LINE_AA)
    if ego is not None:
        egoline = "Egomotion: roll: %f pitch: %f yaw: %f" % (ego['roll'], ego['pitch'], ego['yaw'])
        delta_egoline = "Predicted Delta: roll: %f pitch: %f yaw: %f" % (ego['delta_roll'], ego['delta_pitch'], ego['delta_yaw'])
        delta_actual = "Actual Delta: roll: %f pitch: %f yaw: %f" % (roll - initial['roll'], pitch - initial['pitch'], yaw - initial['yaw'])
        cv2.putText(image, egoline, (20, 80), font, 0.6, (0, 0, 255), 3, cv2.LINE_AA)
        cv2.putText(image, delta_egoline, (20, 110), font, 0.6, (0, 0, 255), 3, cv2.LINE_AA)
        cv2.putText(image, delta_actual, (20, 140), font, 0.6, (0, 0, 255), 3, cv2.LINE_AA)

    if len(target) > 0 and fabs(target['d_x']) <= 1 and fabs(target['d_y'] <= 1):
        t_x = target['t_x']
        t_y = target['t_y']
        d_x = target['d_x']
        d_y = target['d_y']
        cv2.circle(image, (int(d_x * w + w), int(h - h *d_y)), 50, (255, 0, 0), cv2.FILLED)
        cv2.circle(image, (int(t_x + w), int(h - t_y)), 50, (0, 0, 255), cv2.FILLED)
    return image


def get_frame(flight_name, frame_number, data, egomotion=None):
    if int(frame_number) >= len(data):
        print("Invalid frame number, too large")
    else:
        data_pt = data[frame_number]
        img = cv2.imread(join(PRE_PROCESSED_FOLDER, flight_name, data_pt['path']))
        if egomotion is not None:
            egodata = egomotion.get_next()
            print_flight_delta(data_pt, egomotion.initial)
            return create_frame(img, data_pt['x'], data_pt['y'], data_pt['z'], data_pt['roll'], data_pt['pitch'], data_pt['yaw'], data_pt['time'], data_pt['mode'], egodata, egomotion.initial)
        else:
            target ={}
            if data_pt['target']:
                target['t_x'] = data_pt['x_coord']
                target['t_y'] = data_pt['y_coord']
                target['s_x'] = data_pt['send_x']
                target['s_y'] = data_pt['send_y']
                target['d_x'] = data_pt['distance_x']
                target['d_y'] = data_pt['distance_y']
                # target['d_x'] = data_pt['distance_x']
                # target['d_y'] = data_pt['distance_y']
            return create_frame(img, data_pt['x'], data_pt['y'], data_pt['z'], data_pt['roll'], data_pt['pitch'], data_pt['yaw'], data_pt['time'], data_pt['mode'], target)

def load_flight_data(flight_name):
    flight = join(PRE_PROCESSED_FOLDER, flight_name)
    if not isdir(flight):
        print("Flight directory %s Could Not Be Found" % flight)
        return None
    else:
        print("Found flight directory %s. Starting playback." % flight)
        return json.load(open(join(flight, 'data.json')))


def play_flight(flight_name, save):
    flight_data = load_flight_data(flight_name)
    egomotion = None
    video = None
    if flight_data is not None:
        for i in range(len(flight_data)):
            if i == 1:
                pass
                # egomotion = NetResultsParser(join(PRE_PROCESSED_FOLDER, flight_name), flight_data['0'])
            if (i + 1) == len(flight_data):
                print("Last frame. Hit any key to end.")
                frame = get_frame(flight_name, str(i), flight_data, egomotion)
                cv2.imshow('Flight', frame)
                cv2.waitKey(0)
            else:
                frame_time = int(flight_data[str(i + 1)]['time']) - int(flight_data[str(i)]['time'])
                frame_time = 10
                #view_frame(flight_name, str(i), flight_data, frame_time, egoline)
                frame = get_frame(flight_name, str(i), flight_data, egomotion)
                cv2.imshow('Flight', frame)
                cv2.waitKey(frame_time)
            if save:
                if video is None:
                    height, width, _, = frame.shape
                    fourcc = cv2.VideoWriter_fourcc('m', 'p', '4', 'v')
                    video = cv2.VideoWriter(join('pre-processed-data', flight_name, 'output.m4v'), fourcc, 90, (width, height))
                video.write(frame)
    if save:
        video.release()


def print_flight_delta(data_point, initial):
    d_yaw = (data_point['yaw'] - initial['yaw']) * math.pi / 180
    d_roll = (data_point['roll'] - initial['roll']) * math.pi / 180
    d_pitch = (data_point['pitch'] - initial['pitch']) * math.pi / 180
    theta = [d_pitch, d_yaw, d_roll]
    R = euler_angles_to_rot(theta)
    origin = np.array([0,0,1])
    print("--------Pixhawk IMU----------")
    change = np.linalg.norm(origin - np.dot(R, origin))
    print("r_x, r_y, rz: (%f, %f, %f)" % (theta[0], theta[1], theta[2]))
    print(np.dot(R, origin))
    print("Change is: " + str(change))

def euler_angles_to_rot(theta):
    R_x = np.array([[1, 0, 0],
                    [0, math.cos(theta[0]), -math.sin(theta[0])],
                    [0, math.sin(theta[0]), math.cos(theta[0])]
                    ])

    R_y = np.array([[math.cos(theta[1]), 0, math.sin(theta[1])],
                    [0, 1, 0],
                    [-math.sin(theta[1]), 0, math.cos(theta[1])]
                    ])

    R_z = np.array([[math.cos(theta[2]), -math.sin(theta[2]), 0],
                    [math.sin(theta[2]), math.cos(theta[2]), 0],
                    [0, 0, 1]
                    ])

    R = np.dot(R_z, np.dot(R_y, R_x))

    return R
if __name__ == '__main__':
    if len(sys.argv) == 3:
        script, path, save_arg = sys.argv
        save = save_arg == 'capture'
    else:
        save = False
        script, path = sys.argv
    play_flight(path, save)
