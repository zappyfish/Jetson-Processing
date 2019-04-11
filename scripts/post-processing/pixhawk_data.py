import math
import numpy as np


class FlightDataProcessor:

    FILENAME = 'PIXHAWK_DATA.txt'

    def __init__(self, path):
        filename = path + "/" + self.FILENAME
        with open(filename, 'r') as data_file:
            self.content = data_file.readlines()
        self.ind = 0
        self.max_digits = 1
        max_img = len(self.content)
        while max_img >= 10:
            self.max_digits += 1
            max_img = max_img / 10
        self.ind -= 1

        self.alphas = {'accel_x': 0.1, 'accel_y': 0.1, 'accel_z': 0.1}
        # Guess at initial value
        self.accel_lpf = {'accel_x': 0.0, 'accel_y': 0.0, 'accel_z': -9.8}

    def has_next(self):
        return self.ind < len(self.content)

    def get_next(self):
        line = self.content[self.ind].strip()
        self.ind += 1
        tokens = line.split(',')
        data = {'time': int(tokens[0])}
        for i in range(1, len(tokens), 2):
            key = tokens[i]
            val = float(tokens[i + 1])
            if key == 'accel_z':
                val *= -1  # TODO: delet
            if 'accel' in key:
                if 'y' in key:
                    data['z'] = val
                elif 'x' in key:
                    data['x'] = -val
                else:
                    data['y'] = -val # TODO check me for sign
            data[key] = val
        return data

    def get_last(self):
        tmp = self.ind
        self.ind = len(self.content) - 1
        last = self.get_next()
        self.ind = tmp
        return last

    def peek_time(self):
        return int(self.content[self.ind].strip().split(',')[0])
