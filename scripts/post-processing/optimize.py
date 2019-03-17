from kalman_algo import PoseKalmanFilter
from os.path import join
import json
import math

class PKFOptimizer:

    def __init__(self, directory_path, error_min=0.001, error_max=2):
        self.err_min = error_min
        self.err_max = error_max
        self.directory_path = join('pre-processed-data', directory_path)
        self.ground_truth_data = json.load(open(join(self.directory_path, 'data.json')))

    def optimize(self, steps):
        step_size = (self.err_max - self.err_min) / steps
        min_err_degrees = {'roll': 360, 'pitch': 360}
        min_avg_err_degrees = {'roll': 360, 'pitch': 360}
        optimal_process_error = {'roll': self.err_min, 'pitch': self.err_min}
        process_error = self.err_min
        while process_error < self.err_max:
            local_max, local_avg = self.run_step(process_error)
            for key in min_err_degrees:
                if local_max[key] < min_err_degrees[key]:
                    min_err_degrees[key] = local_max[key]
                    optimal_process_error[key] = process_error
                if local_avg[key] < min_avg_err_degrees[key]:
                    min_avg_err_degrees[key] = local_avg[key]
            process_error += step_size
        print("optimal process error:")
        print(optimal_process_error)
        print("Achieved avg: ")
        print(min_avg_err_degrees)
        print("Achieved min: ")
        print(min_err_degrees)
        return optimal_process_error, min_avg_err_degrees, min_err_degrees


    def run_step(self, process_error):
        pkf = PoseKalmanFilter(self.directory_path, process_error=process_error)
        ind = 1
        max_err_degrees = {'roll': 0, 'pitch': 0}
        avg_err_degrees = {'roll': 0, 'pitch': 0}
        while not pkf.is_done():
            pkf_computed, pkf_measurement = pkf.kalman_step() # need to mult by 180/pi
            gt = self.ground_truth_data[str(ind)]
            ind += 1
            for key in avg_err_degrees:
                err = abs(gt[key] - (pkf_computed[key] * 180 / math.pi))
                max_err_degrees[key] = max(max_err_degrees[key], err)
                avg_err_degrees[key] += err
        for key in avg_err_degrees:
            avg_err_degrees[key] = avg_err_degrees[key] / ind
        return max_err_degrees, avg_err_degrees
