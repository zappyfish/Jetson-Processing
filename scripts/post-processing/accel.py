

class FlightDataProcessor:

    FILENAME = 'raw_accel.txt'

    def __init__(self, path):
        filename = path + "/" + self.FILENAME
        with open(filename, 'r') as data_file:
            self.content = data_file.readlines()
        self.alphas = {'accel_x': 0.15, 'accel_y': 0.15, 'accel_z': 0.15}
        # Guess at initial value
        self.accel_lpf = {'accel_x': 0.0, 'accel_y': 0.0, 'accel_z': -9.8}

        self.accels = {}
        for line in self.content:
            s = line.strip().split(",")
            ac = self.get_accel_next(s)




    def get_accel_next(self, splt):
        data = {'time': splt[0]}
        for i in range(1, len(splt), 2):
            data[splt[i]] = int(splt[i + 1])

    def pre_compute(self):
