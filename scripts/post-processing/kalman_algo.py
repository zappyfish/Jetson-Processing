from os.path import join
import json
from s2d_data import S2dDataParser
from math import atan2, sqrt, pow, exp

class PoseKalmanFilter:

    PROCESS_ERROR_COVARIANCE = 0.001 # TODO: fine tune this

    def __init__(self, processed_folder, process_error=PROCESS_ERROR_COVARIANCE):
        self.imu_data = json.load(open(join(processed_folder, 'data.json')))
        self.ind = 0
        self.s2d_data = S2dDataParser(processed_folder)
        self.covariance, self.value = self.get_next_imu()
        self.last_time = float(self.imu_data['0']['time'])
        self.p_error_cov_sqrd = process_error * process_error

    def get_next_imu(self):
        imu = self.imu_data[str(self.ind)]
        self.ind += 1
        accuracy = PoseKalmanFilter.accuracy(imu['accel_x'], imu['accel_y'], imu['accel_z'])
        angles = PoseKalmanFilter.accel_to_euler(imu['accel_x'], imu['accel_y'], imu['accel_z'])
        return accuracy, angles

    @staticmethod
    def accel_to_euler(x, y, z):
        roll = atan2(-y, -z)
        pitch = atan2(x, sqrt((y * y) + (z * z)))
        return {'roll': roll, 'pitch': pitch}  # TODO: delete negative sign

    def is_done(self):
        return self.ind >= len(self.imu_data) or not self.s2d_data.has_next()

    def kalman_step(self):
        s2d_delta = self.s2d_data.get_next()
        prediction = {key: (s2d_delta[key] + self.value[key]) for key in self.value}
        accuracy, measurement = self.get_next_imu()
        cur_time = float(self.imu_data[str(self.ind)]['time'])
        delta_time = cur_time - self.last_time
        self.last_time = cur_time
        self.covariance += (self.p_error_cov_sqrd * delta_time)
        measurement_error = 0.99 / accuracy
        kalman_gain = self.covariance / (self.covariance + measurement_error)
       #  print("step: %d, accuracy squared: %f, kalman gain: %f" % (self.ind, accuracy * accuracy, kalman_gain))
        for key in self.value:
            self.value[key] = prediction[key] + kalman_gain * (measurement[key] - prediction[key])
        self.covariance = (1 - kalman_gain) * self.covariance
        return self.value, measurement

    @staticmethod
    def accuracy(x, y, z):
        diff = abs(9.8 - sqrt((x * x) + (y * y) + (z * z)))
        # return pow((1 / (diff + 1)), 2) * 0.99
        return diff
