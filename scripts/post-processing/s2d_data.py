from os.path import join


class S2dDataParser:

    def __init__(self, processed_data_path):
        x_full_path = join(processed_data_path, 's2d', 'norm', 'egomotion.txt')
        self.data_lines_x = open(x_full_path, 'r').readlines()
        y_full_path = join(processed_data_path, 's2d', 'rot', 'egomotion.txt')
        self.data_lines_y = open(y_full_path, 'r').readlines()
        self.last_line_x = [0.0 for i in range(6)]
        self.last_line_y = [0.0 for i in range(6)]
        self.ind = 0
        self.localization = {'yaw': 0.0, 'roll': 0.0, 'pitch': 0.0,
                             'x': 0.0, 'y': 0.0, 'z': 0.0}

    def has_next(self):
        return self.ind < len(self.data_lines_x)

    def get_next(self):
        x_result = self.get_next_from_line(self.data_lines_x[self.ind].strip(), self.last_line_x)
        y_result = self.get_next_from_line(self.data_lines_y[self.ind].strip(), self.last_line_y)
        self.ind += 1
        self.localization['yaw'] += x_result[4]
        self.localization['roll'] += x_result[5]
        self.localization['pitch'] += y_result[5]
        self.localization['x'] -= x_result[0]
        self.localization['y'] += y_result[0] # TODO: fix
        self.localization['z'] += (x_result[2] + y_result[2]) / 2
        return self.localization

    def get_next_from_line(self, next_line, last_line):
        splt = next_line.split(" ")
        cur = splt[1].split(",")
        nxt = [(last_line[i] + float(cur[i])) / 2 for i in range(6)]
        if self.ind == 0: # First line we have no last line
            for i in range(6):
                nxt[i] = nxt[i] * 2
        last_line = [float(splt[2].split(",")[i]) for i in range(6)]
        # return {'yaw': nxt[1], 'roll': -nxt[2], 'pitch': nxt[0]} # TODO: sign check
        return nxt
