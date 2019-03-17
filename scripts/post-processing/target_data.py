

class TargetDataProcessor:

    FILENAME = "target.txt"

    def __init__(self, path):
        filename = path + "/" + self.FILENAME
        with open(filename, 'r') as data_file:
            self.content = data_file.readlines()
        self.ind = 0

    def has_next(self):
        return self.ind < len(self.content)

    def get_next(self):
        if self.has_next():
            line = self.content[self.ind].strip()
            self.ind += 1
            tokens = line.split(',')
            data = {'time': int(tokens[0])}
            for i in range(1, len(tokens), 2):
                key = tokens[i]
                try:
                    val = float(tokens[i + 1])
                except:
                    val = tokens[i + 1] == "true"
                data[key] = val
            if 'should_land' not in data:
                data['should_land'] = False
            return data
        else:
            return None

    def peek_time(self):
        return int(self.content[self.ind].strip().split(',')[0])
