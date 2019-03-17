class ModeDataProcessor:

    FILENAME = "mode.txt"

    def __init__(self, path):
        filename = path + "/" + self.FILENAME
        with open(filename, 'r') as data_file:
            self.content = data_file.readlines()
        self.ind = 0
        self.mode_times = {}
        mode_time = 0
        self.last_mode = 'manual'
        for data_pt in self.content:
            line = data_pt.strip()
            splt = line.split(',')
            switch_time = int(splt[0])
            mode = splt[2]
            for i in range(mode_time, switch_time):
                self.mode_times[i] = self.last_mode
            mode_time = switch_time
            self.last_mode = mode

    def get_mode_for_time(self, time):
        if time > len(self.mode_times):
            return self.last_mode
        else:
            return self.mode_times[time]

class AlwaysManualMode:

    def __init__(self):
        pass

    def get_mode_for_time(self, time):
        return 'manual'