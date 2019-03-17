import math
import numpy as np

class NetResultsParser:

    FILENAME = "net/poses.txt"

    def __init__(self, path, initial):
        filename = path + "/" + self.FILENAME
        with open(filename, 'r') as data_file:
            self.content = data_file.readlines()
        self.ind = 0
        self.initial = initial

    def get_next(self):
        data = {}
        line = self.content[self.ind].strip()
        self.ind += 1
        tokens = line.split(',')
        # theta = [float(tokens[5]), -float(tokens[4]), float(tokens[3])]
        theta = [float(tokens[3]), float(tokens[4]), float(tokens[5])]
        # print ("delta x: %f" % (theta[0] * 180 / math.pi))
        # print ("delta y: %f" % (theta[1] * 180 / math.pi))
        # print ("delta z: %f" % (theta[2] * 180 / math.pi))
        R = self.euler_angles_to_rot(theta)
        origin = np.array([0, 0, 1])
        change = np.linalg.norm(origin - np.dot(R, origin))
        print("---------Camera---------")
        print("r_x, r_y, r_z: (%f, %f, %f)" % (theta[0], theta[1], theta[2]))
        print(np.dot(R, origin))
        print("Change is: " + str(change))
        yaw = math.atan2(R[1, 0], R[0, 0])
        pitch = math.atan2(-R[2, 0], math.sqrt(math.pow(R[2, 1], 2) + math.pow(R[2, 2], 2)))
        roll = math.atan2(R[2, 1], R[2, 2])
        data['yaw'] = yaw * 180 / math.pi
        data['pitch'] = pitch * 180 / math.pi
        data['roll'] = roll * 180 / math.pi
        k = data.keys()
        for v in k:
            data['delta_' + v] = data[v]
            data[v] += self.initial[v]
        return data

    def euler_angles_to_rot(self, theta):
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