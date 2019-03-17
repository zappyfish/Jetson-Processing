from os.path import join

class S2dDataParser:

    def __init__(self, processed_data_path):
        full_path = join(processed_data_path, 'net', 's2d', 'egomotion.txt')
        self.data_lines = open(full_path, 'r').readlines()
        self.last_line_rot = [0.0, 0.0, 0.0]
        self.ind = 0

    def has_next(self):
        return self.ind < len(self.data_lines)

    def get_next(self):
        next_line = self.data_lines[self.ind].strip()
        self.ind += 1
        splt = next_line.split(" ")
        cur = splt[1].split(",")
        nxt = [(self.last_line_rot[i] + float(cur[i + 3])) / 2 for i in range(3)]
        self.last_line_rot = [float(splt[2].split(",")[i + 3]) for i in range(3)]
        return {'yaw': nxt[1], 'roll': nxt[2], 'pitch': nxt[0]}