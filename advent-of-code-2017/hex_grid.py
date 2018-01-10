# Day 11

# Hex grid can be implemented as a regular grid, but with every other column
# shifted up a half position
# o0   o0   o0
#   x0   x0
# o1   o1   o1
#   x1   x1
# o2   o2   o2
#   x2   x2
# o3   o3   o3
# In an x row (even), south+direction is +1 vertical, north+direction is neutral
# In an o row (odd), south+direction is neutral, north+direction is -1 vertical

X = 1
Y = 0

class HexGrid(object):
    directions = {
        'n': [-1, 0],
        'ne': [-1, 1],
        'se': [1, 1],
        's': [1, 0],
        'sw': [1, -1],
        'nw': [-1, -1],
    }

    def get_steps_from_center(self, step_list):
        num_steps = 0
        max_num_steps = 0
        total_direction = [0,0]

        for step_raw in step_list:
            step = self.parse_raw_direction(step_raw)
            total_direction = self.hex_step(total_direction, step)

            num_steps = self.get_num_steps([0,0], total_direction)
            if num_steps > max_num_steps:
                max_num_steps = num_steps
            
        # configure number of steps based on even and odd
        return max_num_steps

    def parse_raw_direction(self, step):
        return self.directions[step]

    def get_num_steps(self, start, end):
        total_steps = 0
        diff = [0, 0]
        while start != end:
            step_raw = ''
            diff = [end[Y] - start[Y], end[X] - start[X]]
            if start[X] == end[X]:
                # vertical direction
                total_steps += abs(diff[Y])
                start = [end[Y], start[X]]
            elif start[Y] == end[Y]:
                # horizontal direction
                if start[X] % 2 == 0:
                    if diff[X] < 0:
                        step_raw = 'nw'
                    elif diff[X] > 0:
                        step_raw = 'ne'
                else:
                    if diff[X] < 0:
                        step_raw = 'sw'
                    elif diff[X] > 0:
                        step_raw = 'se'
            
                step = self.parse_raw_direction(step_raw)
                start = self.hex_step(start, step)
                total_steps += 1
            else:
                # diagonal direction
                if diff[Y] > 0 and diff[X] > 0:
                    step_raw = 'se'
                elif diff[Y] > 0 and diff[X] < 0:
                    step_raw = 'sw'
                elif diff[Y] < 0 and diff[X] < 0:
                    step_raw = 'nw'
                elif diff[Y] < 0 and diff[X] > 0:
                    step_raw = 'ne'
                step = self.parse_raw_direction(step_raw)
                start = self.hex_step(start, step)
                total_steps += 1

        return total_steps

    def hex_step(self, vector, step):
        if step[X] != 0:
            if step[Y] == -1 and vector[X] % 2 == 0:
                step = [0, step[X]]
            elif step[Y] == 1 and vector[X] % 2 == 1:
                step = [0, step[X]]
        vector = [vector[Y] + step[Y], vector[X] + step[X]]
        return vector


pset_input = """ne,n,ne,s,nw,s,s,sw,sw,sw,sw,sw,nw,nw,sw,nw,nw,n,nw,nw,nw,nw,nw,s,n,nw,s,n,n,nw,n,n,se,\
n,n,n,s,n,n,n,n,sw,se,n,n,ne,s,ne,ne,nw,ne,n,ne,ne,ne,ne,ne,s,ne,ne,s,se,ne,ne,ne,ne,ne,nw,ne,se,ne,ne,n,\
ne,ne,se,ne,ne,se,se,se,se,se,n,se,ne,se,se,ne,ne,se,sw,sw,se,se,s,sw,se,se,se,s,n,se,se,s,se,s,se,se,se,\
se,se,se,n,s,s,nw,se,s,s,nw,s,se,se,s,s,s,sw,s,s,se,n,ne,s,s,s,s,s,s,nw,s,n,s,n,se,s,s,sw,s,ne,s,s,s,s,s,\
nw,ne,s,s,sw,s,s,s,s,se,n,nw,s,s,s,s,s,sw,s,n,sw,s,s,sw,sw,s,sw,sw,ne,sw,sw,s,ne,s,sw,sw,sw,sw,s,sw,se,sw,\
sw,s,sw,se,s,sw,sw,sw,se,sw,sw,sw,sw,s,sw,s,sw,s,sw,sw,ne,sw,sw,ne,sw,sw,s,sw,sw,sw,ne,sw,sw,sw,sw,se,se,\
sw,nw,sw,sw,nw,sw,nw,nw,sw,sw,sw,sw,nw,sw,nw,sw,sw,nw,sw,n,sw,nw,sw,s,s,se,nw,s,nw,sw,nw,nw,sw,nw,s,nw,se\
,s,sw,se,nw,nw,n,sw,sw,nw,sw,nw,nw,nw,ne,sw,ne,se,sw,sw,ne,nw,sw,nw,nw,nw,ne,nw,nw,sw,nw,sw,nw,sw,nw,nw,nw\
,nw,nw,ne,nw,nw,nw,nw,nw,nw,nw,ne,nw,nw,nw,nw,nw,nw,nw,ne,ne,nw,s,sw,nw,nw,nw,se,nw,se,se,nw,nw,nw,n,nw,nw\
,ne,nw,nw,nw,ne,nw,n,sw,nw,ne,nw,sw,nw,n,nw,s,nw,n,nw,nw,nw,nw,nw,n,n,sw,nw,nw,n,nw,n,nw,nw,nw,nw,se,n,nw,\
nw,nw,ne,nw,n,nw,n,nw,n,nw,nw,nw,nw,n,nw,sw,nw,n,n,se,n,n,n,nw,nw,n,nw,n,ne,nw,nw,nw,nw,n,s,nw,s,nw,nw,n,\
nw,n,n,n,n,s,nw,nw,n,n,se,nw,nw,nw,s,n,n,sw,sw,n,n,nw,n,n,n,nw,sw,n,ne,s,n,n,n,n,nw,n,nw,n,n,n,sw,n,n,n,n,\
nw,n,n,n,n,n,ne,n,se,n,n,n,n,n,n,ne,n,n,n,se,n,n,n,n,n,ne,n,n,n,ne,nw,n,n,n,n,n,ne,ne,n,n,n,n,n,n,n,n,n,n,\
n,nw,se,n,ne,n,sw,n,n,n,n,ne,ne,n,ne,n,n,s,n,sw,nw,n,ne,ne,nw,s,se,ne,n,ne,n,n,ne,se,n,sw,ne,n,n,se,s,ne,n\
e,n,n,n,ne,ne,n,se,s,n,n,n,n,n,ne,s,nw,ne,n,n,n,ne,ne,sw,ne,ne,n,ne,ne,ne,ne,sw,ne,n,n,ne,n,ne,n,n,n,s,n,n\
,ne,n,ne,n,ne,ne,ne,nw,ne,sw,s,ne,n,ne,ne,ne,n,n,ne,ne,ne,se,nw,ne,n,ne,ne,n,ne,ne,ne,ne,ne,n,n,n,nw,ne,ne\
,se,n,n,n,n,ne,n,ne,ne,nw,ne,ne,n,ne,ne,ne,ne,ne,nw,n,ne,se,ne,ne,ne,ne,nw,ne,ne,ne,ne,ne,ne,ne,se,se,ne,n\
e,ne,ne,ne,ne,ne,n,nw,ne,ne,ne,ne,ne,nw,ne,ne,ne,ne,nw,ne,ne,nw,se,ne,ne,ne,ne,ne,ne,ne,ne,nw,ne,ne,ne,ne,n\
e,se,ne,sw,ne,ne,se,ne,sw,ne,ne,ne,ne,ne,ne,se,ne,ne,ne,ne,ne,nw,ne,n,ne,s,ne,ne,se,ne,n,ne,sw,se,ne,ne,sw,\
se,ne,se,se,ne,ne,se,se,ne,s,ne,s,ne,n,ne,ne,ne,ne,sw,ne,ne,se,ne,ne,ne,se,ne,se,ne,ne,ne,se,nw,ne,ne,ne,n\
e,ne,s,ne,s,se,ne,ne,se,n,ne,se,se,se,ne,ne,ne,ne,ne,se,sw,ne,ne,se,se,se,ne,ne,se,se,se,ne,se,s,ne,se,sw,\
ne,se,ne,se,ne,se,ne,se,se,n,se,se,n,ne,se,se,se,ne,n,ne,se,ne,n,n,ne,s,ne,se,n,ne,ne,se,nw,se,se,ne,se,ne\
,sw,s,n,se,nw,ne,se,se,ne,ne,se,se,se,se,se,ne,ne,ne,ne,se,se,se,se,ne,se,ne,se,se,n,se,sw,sw,se,se,se,se\
,se,se,se,se,ne,se,nw,se,se,se,ne,ne,n,se,ne,se,se,ne,se,s,n,s,ne,se,se,ne,se,ne,se,se,se,ne,se,se,se,se,\
ne,se,se,ne,se,se,ne,se,se,se,se,se,ne,ne,ne,se,se,se,se,se,n,se,s,nw,se,n,s,se,se,se,se,se,se,sw,se,sw,se\
,se,sw,se,nw,se,se,se,se,se,ne,se,se,se,ne,se,se,s,se,se,se,se,nw,s,nw,se,se,se,se,se,s,se,ne,se,n,se,se,s\
e,sw,se,n,se,se,se,se,se,se,sw,se,se,n,se,se,se,se,se,n,sw,s,se,se,n,se,se,se,se,sw,se,se,se,nw,se,se,se,se\
,se,s,s,sw,se,sw,se,se,s,se,se,se,s,ne,se,sw,se,s,se,se,ne,se,se,se,se,s,se,se,se,se,s,se,se,s,s,s,se,sw,s\
e,se,se,n,se,sw,s,s,se,sw,se,s,s,s,se,ne,se,se,se,s,se,sw,s,s,se,se,n,sw,s,n,sw,s,se,se,s,s,s,n,se,se,se\
,s,se,se,se,s,se,se,se,se,nw,se,n,se,s,se,se,s,se,s,nw,se,s,ne,se,se,s,sw,se,s,n,se,n,s,se,se,se,ne,se,s,\
s,s,ne,se,se,se,se,n,se,s,se,se,s,se,n,se,se,s,n,se,s,s,se,s,nw,se,s,s,se,se,s,s,s,nw,se,se,se,se,s,nw,se\
,s,se,s,se,se,s,se,s,s,n,s,s,se,s,nw,s,se,s,se,s,s,ne,se,s,s,n,se,s,s,s,n,se,se,s,s,ne,s,se,s,s,s,s,se,s,\
se,sw,s,s,s,se,nw,s,s,nw,se,s,s,s,s,s,s,se,sw,sw,nw,s,sw,s,s,s,s,sw,s,s,s,s,sw,s,s,s,s,s,s,se,s,s,s,se,s,s\
e,s,n,s,s,s,se,s,s,s,s,ne,s,s,s,se,s,s,s,s,s,s,s,s,sw,s,s,s,s,se,s,s,ne,s,se,s,se,nw,s,nw,n,n,s,se,s,s,s,n\
,s,s,se,s,n,s,s,s,sw,s,nw,s,sw,nw,ne,s,s,s,s,s,s,s,s,s,sw,s,nw,s,s,s,s,s,s,s,s,nw,s,n,s,s,s,s,ne,s,s,sw,s,\
s,s,n,n,s,s,sw,s,s,se,sw,s,s,s,nw,s,nw,sw,s,n,s,s,se,n,s,s,s,s,sw,s,s,s,nw,s,s,s,s,s,s,s,s,s,s,s,ne,s,nw,s\
,s,s,sw,s,s,s,s,s,s,s,nw,n,n,s,se,s,s,s,s,sw,s,s,s,s,s,nw,sw,s,s,sw,s,s,se,s,s,s,s,s,s,s,sw,s,s,s,s,sw,s,s\
,s,s,n,s,nw,sw,s,s,s,s,s,s,s,sw,s,s,sw,s,sw,s,sw,s,sw,s,n,s,sw,s,n,sw,s,s,s,s,sw,s,s,s,ne,sw,s,s,s,s,s,s,s\
,sw,s,s,nw,ne,s,s,se,sw,sw,s,s,n,s,s,s,s,s,s,sw,sw,ne,s,s,s,s,s,s,s,s,s,sw,s,s,s,sw,sw,sw,sw,s,s,sw,ne,sw,\
sw,s,s,s,s,ne,sw,s,se,s,s,s,s,s,s,sw,s,sw,s,ne,sw,sw,s,s,n,sw,s,s,sw,se,s,nw,s,sw,s,s,sw,s,sw,sw,s,sw,sw\
,sw,sw,s,sw,s,s,s,s,s,s,s,sw,s,s,sw,s,sw,n,s,s,s,sw,ne,s,sw,s,sw,n,n,sw,sw,sw,s,nw,nw,sw,sw,n,n,s,sw,se,\
sw,sw,sw,sw,sw,s,s,s,sw,sw,s,s,sw,sw,n,s,s,sw,s,sw,sw,sw,nw,sw,s,sw,sw,se,sw,s,sw,sw,sw,s,s,sw,s,s,s,s,s\
,nw,s,sw,n,s,sw,n,sw,nw,sw,sw,s,sw,n,s,s,n,sw,sw,s,ne,s,sw,s,sw,sw,s,sw,s,s,nw,s,s,nw,s,sw,n,s,sw,sw,sw,\
sw,sw,sw,s,se,s,n,s,s,s,s,sw,sw,ne,nw,sw,s,nw,sw,nw,n,sw,s,sw,se,sw,s,sw,n,s,s,sw,sw,n,sw,sw,sw,s,sw,n,s\
w,sw,sw,n,nw,s,sw,sw,s,sw,s,se,sw,sw,n,sw,sw,sw,ne,sw,s,s,s,sw,sw,s,se,se,sw,sw,s,s,sw,ne,nw,sw,s,sw,sw,\
sw,s,sw,sw,sw,sw,sw,sw,n,sw,sw,s,s,sw,sw,sw,sw,s,sw,sw,s,sw,s,s,sw,sw,sw,sw,sw,sw,sw,sw,sw,sw,s,n,nw,sw,\
n,sw,sw,sw,se,sw,nw,sw,se,sw,sw,sw,ne,sw,sw,sw,sw,sw,n,sw,sw,sw,sw,s,se,s,sw,sw,s,sw,sw,nw,sw,sw,sw,sw,s\
,se,sw,sw,sw,sw,sw,sw,sw,sw,sw,sw,sw,sw,sw,nw,sw,sw,sw,sw,sw,sw,sw,sw,sw,n,sw,sw,sw,sw,nw,sw,ne,sw,sw,sw\
,sw,n,sw,n,sw,nw,sw,sw,ne,s,sw,sw,sw,sw,nw,sw,se,sw,sw,sw,sw,sw,sw,sw,sw,nw,s,n,nw,nw,sw,nw,sw,sw,sw,nw,\
sw,sw,n,sw,se,nw,sw,ne,sw,sw,sw,nw,sw,sw,sw,sw,sw,se,sw,sw,sw,nw,n,nw,ne,sw,sw,sw,sw,sw,sw,sw,sw,nw,sw,s\
w,sw,sw,se,sw,n,sw,sw,sw,sw,sw,sw,sw,s,n,sw,sw,sw,sw,sw,s,sw,sw,sw,sw,sw,sw,sw,nw,sw,sw,sw,nw,sw,sw,s,nw\
,sw,nw,sw,sw,ne,sw,nw,sw,se,sw,nw,s,nw,sw,nw,sw,nw,sw,sw,sw,sw,sw,n,sw,ne,nw,ne,sw,sw,sw,sw,s,nw,sw,sw,s\
w,ne,s,sw,sw,sw,sw,sw,sw,sw,sw,sw,sw,sw,sw,sw,nw,sw,sw,sw,sw,sw,sw,sw,nw,sw,sw,nw,ne,sw,s,sw,se,s,nw,sw,\
sw,sw,ne,sw,sw,n,sw,sw,sw,nw,nw,ne,sw,sw,sw,sw,sw,se,sw,nw,se,sw,se,sw,sw,sw,sw,sw,ne,se,sw,sw,s,sw,sw,n\
w,sw,sw,sw,sw,sw,sw,sw,nw,nw,nw,ne,nw,sw,sw,n,sw,se,nw,sw,sw,n,nw,nw,se,n,s,se,se,ne,sw,sw,nw,sw,sw,sw,s\
w,sw,nw,sw,ne,n,sw,sw,se,se,sw,nw,nw,nw,nw,sw,nw,ne,sw,nw,sw,nw,se,nw,n,sw,sw,sw,sw,sw,nw,sw,sw,nw,sw,sw\
,nw,se,s,sw,ne,nw,ne,nw,sw,sw,sw,n,sw,sw,nw,sw,sw,nw,se,sw,sw,s,sw,nw,nw,sw,sw,nw,sw,se,sw,sw,nw,nw,n,sw\
,nw,s,nw,sw,se,s,nw,sw,sw,sw,sw,sw,ne,nw,sw,nw,nw,sw,nw,se,nw,se,s,sw,sw,nw,nw,sw,sw,sw,sw,sw,nw,sw,nw,n\
e,nw,nw,sw,se,sw,nw,nw,sw,nw,sw,sw,sw,nw,ne,ne,sw,sw,s,sw,nw,s,nw,sw,nw,sw,sw,sw,se,se,sw,sw,sw,sw,sw,sw\
,sw,sw,se,n,nw,nw,nw,sw,sw,n,sw,s,sw,nw,nw,nw,s,sw,sw,n,sw,n,ne,nw,sw,nw,nw,sw,sw,s,sw,sw,nw,nw,nw,sw,sw\
,nw,nw,nw,nw,sw,se,sw,nw,nw,sw,sw,nw,sw,sw,sw,sw,nw,nw,ne,se,nw,n,sw,nw,sw,se,sw,nw,ne,nw,sw,sw,nw,nw,nw\
,sw,nw,nw,nw,sw,nw,sw,ne,nw,s,sw,nw,nw,nw,sw,sw,n,nw,n,nw,nw,sw,s,nw,nw,sw,nw,nw,sw,nw,nw,sw,nw,sw,nw,se\
,sw,nw,sw,sw,sw,sw,nw,sw,nw,sw,nw,ne,nw,nw,nw,nw,se,sw,sw,nw,sw,nw,sw,sw,nw,nw,nw,sw,sw,sw,s,nw,sw,n,sw,\
sw,nw,nw,nw,sw,sw,nw,nw,sw,sw,nw,nw,se,ne,nw,se,s,nw,s,nw,sw,sw,sw,nw,sw,nw,n,nw,sw,sw,nw,nw,nw,nw,nw,nw\
,sw,nw,sw,ne,nw,nw,s,se,nw,sw,sw,nw,nw,nw,nw,nw,sw,nw,sw,ne,ne,nw,nw,sw,s,nw,nw,nw,nw,nw,nw,sw,nw,nw,nw\
,ne,nw,se,nw,n,nw,sw,sw,sw,se,ne,nw,sw,sw,nw,nw,nw,nw,sw,sw,s,n,ne,nw,nw,nw,sw,nw,sw,nw,nw,nw,nw,ne,nw,\
nw,nw,nw,nw,se,nw,s,nw,sw,n,sw,sw,nw,se,nw,nw,nw,sw,nw,nw,sw,sw,sw,sw,nw,nw,n,nw,nw,ne,nw,nw,nw,s,nw,sw\
,ne,nw,nw,nw,nw,nw,nw,nw,nw,s,nw,sw,se,n,nw,nw,sw,nw,n,nw,nw,nw,sw,nw,n,sw,nw,nw,nw,nw,sw,nw,sw,nw,nw,s\
,s,nw,nw,nw,n,nw,nw,nw,nw,sw,se,nw,sw,nw,nw,nw,nw,se,nw,n,se,nw,n,nw,nw,s,sw,nw,nw,nw,n,nw,nw,nw,nw,nw,\
nw,nw,nw,nw,ne,nw,n,n,sw,nw,sw,nw,nw,nw,ne,nw,nw,s,nw,nw,nw,nw,nw,nw,sw,nw,sw,nw,nw,ne,sw,nw,s,nw,nw,n\
e,nw,nw,nw,nw,nw,nw,nw,nw,nw,nw,nw,nw,nw,nw,nw,nw,ne,s,nw,nw,se,sw,nw,nw,nw,s,nw,nw,nw,nw,nw,nw,nw,nw,n\
w,nw,nw,nw,sw,nw,nw,nw,nw,nw,se,nw,ne,s,sw,nw,n,nw,nw,nw,se,nw,nw,sw,nw,sw,nw,nw,n,ne,nw,nw,nw,nw,nw,nw\
,nw,s,n,sw,s,nw,ne,s,nw,nw,sw,ne,n,nw,nw,nw,nw,nw,nw,sw,s,nw,nw,sw,nw,n,nw,nw,n,nw,nw,nw,nw,nw,n,sw,nw,\
nw,nw,sw,s,nw,se,nw,nw,nw,s,nw,nw,nw,nw,nw,nw,nw,nw,nw,nw,n,nw,nw,sw,nw,n,nw,nw,nw,n,nw,nw,nw,nw,nw,nw,n\
w,nw,nw,nw,nw,nw,nw,nw,n,nw,n,n,nw,se,nw,nw,nw,n,nw,nw,nw,sw,nw,nw,n,nw,nw,nw,nw,s,nw,s,nw,nw,s,nw,ne,s,\
nw,nw,nw,s,n,nw,nw,nw,nw,nw,nw,n,nw,nw,nw,sw,nw,n,nw,nw,nw,nw,s,n,nw,se,nw,nw,n,nw,nw,n,nw,nw,nw,n,n,nw,\
ne,n,nw,se,nw,nw,se,nw,nw,nw,n,se,nw,nw,nw,nw,nw,nw,nw,nw,n,se,nw,nw,ne,n,nw,n,nw,se,nw,n,nw,n,nw,nw,nw,\
nw,nw,se,nw,nw,nw,nw,nw,ne,nw,n,n,nw,nw,sw,se,nw,nw,nw,nw,nw,nw,s,s,se,n,nw,n,nw,se,nw,nw,nw,nw,nw,sw,n,\
nw,nw,nw,nw,n,s,n,nw,nw,n,nw,nw,n,nw,nw,nw,nw,ne,nw,nw,nw,nw,nw,nw,nw,nw,sw,nw,nw,ne,s,ne,nw,nw,nw,nw,sw\
,nw,n,nw,nw,se,n,nw,nw,n,ne,nw,nw,n,n,nw,n,n,nw,nw,nw,nw,ne,nw,nw,nw,n,nw,n,n,nw,n,nw,ne,nw,nw,nw,n,nw,s\
e,n,nw,ne,n,nw,n,nw,nw,nw,nw,s,n,n,nw,n,se,nw,nw,nw,nw,ne,n,nw,nw,nw,n,nw,nw,s,nw,n,sw,nw,nw,s,n,n,nw,nw\
,nw,n,n,sw,n,se,nw,nw,nw,nw,s,nw,n,nw,ne,nw,nw,nw,nw,s,sw,n,nw,n,nw,ne,nw,nw,n,n,nw,n,sw,n,nw,nw,nw,nw,nw\
,n,nw,nw,n,n,n,s,n,sw,ne,n,nw,ne,nw,n,n,nw,ne,se,nw,nw,s,nw,n,sw,nw,sw,n,nw,ne,nw,nw,nw,nw,s,nw,n,s,nw,n,\
n,ne,sw,nw,nw,nw,nw,n,n,nw,sw,n,n,nw,nw,ne,n,s,nw,n,n,nw,se,n,n,n,nw,n,nw,n,nw,nw,nw,nw,n,n,nw,n,n,n,n,n\
w,n,nw,nw,nw,sw,nw,n,ne,n,se,ne,n,nw,nw,n,n,s,n,n,nw,n,ne,s,n,nw,sw,se,n,n,nw,nw,n,n,n,n,n,s,n,n,n,sw,n,\
s,nw,n,n,nw,nw,n,sw,n,nw,nw,nw,n,nw,n,nw,nw,nw,nw,sw,nw,nw,s,sw,s,n,n,nw,s,nw,nw,se,nw,nw,nw,n,se,n,n,nw\
,nw,n,n,ne,n,sw,s,n,nw,nw,nw,n,s,n,nw,nw,nw,n,n,n,n,n,nw,nw,nw,nw,nw,n,nw,nw,se,n,n,se,nw,ne,n,sw,nw,n,n\
,se,s,se,n,nw,sw,nw,n,s,nw,n,nw,nw,nw,se,nw,nw,nw,s,n,n,nw,s,n,n,nw,n,n,n,n,n,n,nw,n,n,nw,n,n,n,n,n,nw,s\
,sw,nw,ne,n,n,nw,nw,nw,n,nw,nw,n,n,n,nw,n,nw,nw,n,n,n,nw,n,nw,n,nw,nw,nw,n,n,nw,nw,nw,n,ne,s,nw,nw,nw,nw\
,nw,n,n,nw,n,nw,n,n,nw,nw,n,nw,nw,n,se,n,n,nw,n,ne,nw,n,ne,ne,sw,n,n,n,n,nw,n,se,nw,nw,n,se,n,nw,nw,n,s,\
n,n,nw,nw,nw,sw,n,ne,s,n,n,n,nw,nw,n,sw,n,nw,nw,n,n,nw,n,ne,n,nw,nw,nw,nw,nw,n,n,n,nw,n,n,n,nw,nw,n,n,n,\
n,n,n,nw,nw,s,nw,n,n,n,n,nw,nw,n,n,sw,n,sw,n,n,nw,n,sw,n,n,n,nw,nw,n,ne,n,n,nw,se,s,n,n,n,n,n,n,n,n,n,n,\
n,n,n,n,se,se,ne,n,n,n,n,n,nw,nw,n,n,ne,n,nw,n,n,sw,n,nw,n,n,s,se,n,n,n,nw,n,n,n,n,n,n,nw,n,n,sw,s,n,s,n\
,n,n,n,sw,n,nw,n,n,se,n,n,n,sw,n,ne,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,nw,n,n,n,n,n,n,nw,n,n,n,n,nw,n,n,n,nw,n\
w,nw,n,nw,n,n,se,ne,n,n,nw,n,sw,n,n,n,n,n,s,nw,n,n,n,n,n,nw,nw,sw,n,n,sw,se,n,n,n,n,n,n,n,n,n,n,n,n,n,ne,\
n,n,nw,nw,sw,n,n,n,s,se,n,sw,n,n,n,n,ne,n,nw,n,n,n,n,sw,n,sw,n,n,n,se,n,nw,nw,s,ne,n,n,n,s,n,n,se,n,n,n,n\
w,n,n,n,n,n,sw,n,s,n,n,s,n,n,n,n,ne,nw,n,n,n,nw,n,n,n,nw,nw,nw,n,n,nw,n,n,n,n,n,n,n,n,n,n,n,n,n,ne,nw,n,n\
,sw,ne,n,n,sw,nw,n,n,n,n,nw,n,n,n,n,n,n,nw,nw,n,n,n,n,s,s,n,n,n,n,n,n,nw,sw,n,n,n,n,n,se,n,n,ne,se,n,n,se\
,n,n,n,n,nw,n,s,n,n,s,n,n,n,n,sw,n,n,n,n,n,n,n,n,n,nw,n,n,n,n,n,se,n,n,se,n,n,n,n,n,n,n,n,n,se,n,n,n,n,n\
,s,n,n,n,n,n,ne,n,n,n,n,n,ne,n,n,nw,n,n,n,s,n,n,n,n,ne,n,se,ne,n,nw,n,n,sw,s,n,nw,n,n,ne,n,sw,n,ne,n,n,n\
,n,s,sw,n,n,n,sw,n,ne,n,n,n,n,s,nw,n,n,n,n,n,n,ne,n,se,ne,n,se,n,n,s,nw,n,n,n,nw,n,n,n,ne,n,n,n,n,n,n,ne\
,n,n,nw,n,n,sw,sw,s,n,n,n,n,n,ne,n,sw,n,n,n,n,n,n,n,se,ne,nw,n,n,n,n,n,n,n,se,n,n,nw,ne,n,n,n,ne,ne,nw,n\
,n,n,n,ne,n,n,nw,n,ne,n,n,n,ne,n,n,n,ne,n,n,n,nw,n,s,ne,n,ne,n,n,n,n,nw,se,se,n,ne,n,n,ne,n,se,n,n,n,n,n\
,se,s,n,n,n,n,n,n,n,n,sw,n,n,n,n,n,n,n,n,n,n,n,n,ne,sw,se,n,se,n,n,n,se,ne,ne,n,n,n,se,nw,n,s,ne,n,n,s,n\
,n,n,n,ne,ne,sw,n,n,n,n,n,n,n,n,n,se,n,nw,n,n,n,ne,n,s,n,nw,n,nw,n,ne,n,ne,n,ne,n,n,ne,ne,n,s,n,ne,n,n,n\
,sw,n,n,n,se,n,n,n,n,nw,n,n,sw,n,n,n,n,n,n,sw,n,n,sw,n,n,ne,n,n,n,n,n,ne,n,n,n,n,nw,n,sw,n,n,n,n,ne,n,n,\
n,sw,n,n,n,n,n,n,ne,ne,n,n,n,ne,se,sw,sw,n,n,n,n,ne,s,se,n,n,sw,ne,sw,n,nw,n,sw,n,n,sw,n,ne,ne,n,n,ne,ne\
,sw,n,s,n,n,n,ne,ne,n,n,n,n,n,ne,n,ne,nw,ne,n,n,ne,n,ne,n,n,n,n,ne,n,n,n,n,n,n,n,n,n,n,n,n,ne,sw,se,n,n,\
n,nw,n,ne,s,sw,n,ne,sw,n,n,ne,n,sw,n,n,ne,ne,n,ne,n,n,ne,ne,n,n,n,n,ne,ne,ne,n,n,ne,nw,n,n,n,se,sw,ne,ne\
,n,n,n,n,ne,n,ne,nw,n,s,n,n,se,n,n,n,nw,ne,ne,n,s,se,ne,se,ne,nw,ne,n,n,n,n,ne,ne,n,n,n,ne,nw,se,ne,s,ne\
,sw,n,n,nw,ne,sw,ne,n,ne,ne,ne,n,n,se,nw,nw,ne,ne,ne,ne,ne,sw,ne,n,ne,se,n,sw,ne,ne,ne,ne,n,n,ne,n,ne,n,\
nw,ne,ne,ne,ne,se,n,se,ne,ne,ne,n,n,n,n,sw,n,n,sw,ne,ne,ne,n,s,ne,n,n,ne,ne,ne,sw,n,n,ne,n,sw,ne,s,n,n,n\
e,n,ne,n,ne,n,sw,ne,n,ne,n,n,n,n,n,n,ne,ne,n,s,ne,s,n,n,n,n,n,ne,ne,ne,ne,sw,ne,s,n,ne,se,ne,ne,ne,ne,ne\
,nw,sw,ne,nw,se,n,ne,n,n,n,n,ne,ne,ne,s,ne,n,ne,n,n,n,n,n,nw,ne,ne,n,s,n,ne,se,n,ne,n,n,n,ne,ne,n,ne,n,n\
e,nw,n,ne,n,n,ne,n,n,ne,n,n,n,ne,nw,n,ne,n,ne,n,ne,n,n,ne,s,ne,n,n,n,ne,ne,n,n,ne,n,n,ne,ne,n,n,ne,nw,nw\
,nw,n,n,n,n,n,n,ne,n,ne,ne,n,n,n,se,n,n,n,s,n,n,n,n,ne,n,n,n,ne,ne,ne,n,ne,ne,n,ne,n,n,ne,ne,ne,ne,n,ne,\
ne,n,n,s,ne,n,ne,n,n,n,ne,ne,n,ne,ne,sw,ne,ne,n,ne,nw,n,ne,n,n,ne,n,nw,s,ne,se,ne,ne,ne,n,se,ne,se,se,ne\
,ne,ne,ne,n,ne,n,nw,n,n,n,n,n,s,n,ne,n,sw,s,n,ne,n,ne,ne,n,n,ne,n,se,se,ne,ne,ne,ne,ne,ne,nw,nw,n,ne,n,n,\
n,ne,ne,n,n,n,ne,ne,ne,ne,ne,n,ne,ne,n,n,ne,n,s,sw,n,ne,ne,n,n,n,sw,n,ne,ne,ne,n,ne,ne,ne,ne,ne,ne,ne,n,\
ne,s,sw,n,n,n,ne,n,ne,nw,se,n,n,n,n,n,ne,n,ne,n,n,s,sw,ne,n,ne,n,n,n,n,n,ne,ne,n,n,ne,n,s,n,ne,ne,se,ne,\
n,se,n,n,n,ne,ne,n,n,ne,ne,ne,n,n,ne,ne,n,ne,ne,ne,sw,ne,ne,ne,ne,s,s,n,ne,n,ne,s,ne,ne,s,ne,se,ne,ne,ne,\
ne,n,n,ne,ne,ne,se,se,ne,n,ne,n,s,n,se,nw,ne,sw,ne,n,s,ne,ne,ne,sw,n,ne,ne,ne,nw,n,se,ne,ne,ne,s,s,n,ne,\
ne,n,ne,se,ne,ne,n,n,n,n,ne,ne,ne,ne,ne,ne,nw,s,sw,sw,ne,ne,sw,ne,ne,n,se,ne,ne,sw,n,ne,s,ne,ne,n,n,ne,s\
w,ne,ne,ne,ne,ne,n,ne,ne,ne,ne,ne,ne,n,ne,ne,ne,n,ne,ne,s,ne,ne,ne,se,ne,s,ne,nw,n,ne,ne,ne,s,ne,ne,ne,n\
e,ne,ne,ne,n,ne,n,n,n,ne,se,n,ne,ne,n,n,ne,n,n,ne,ne,ne,n,ne,ne,ne,ne,n,n,n,ne,ne,ne,ne,ne,ne,ne,n,se,n,n\
w,ne,s,sw,n,ne,n,n,ne,n,ne,ne,ne,ne,ne,ne,ne,n,se,ne,ne,ne,se,se,ne,nw,ne,ne,ne,ne,n,ne,ne,ne,ne,n,n,ne,\
s,n,sw,nw,n,nw,ne,ne,n,ne,ne,ne,ne,s,ne,n,ne,ne,ne,ne,ne,ne,n,ne,n,ne,se,ne,ne,n,ne,n,ne,n,ne,ne,ne,ne,s\
w,ne,ne,ne,ne,ne,n,ne,sw,ne,ne,ne,ne,ne,ne,s,n,ne,n,ne,ne,ne,nw,ne,ne,ne,n,n,ne,s,n,n,n,ne,ne,nw,n,ne,n,\
sw,ne,ne,ne,n,ne,ne,ne,ne,s,n,ne,ne,ne,ne,ne,ne,ne,ne,ne,ne,n,n,se,ne,n,nw,ne,s,ne,ne,ne,sw,n,se,nw,se,ne\
,ne,ne,ne,se,n,nw,ne,ne,ne,ne,s,nw,ne,ne,ne,s,ne,se,ne,ne,ne,ne,nw,ne,ne,n,ne,ne,ne,ne,n,ne,ne,ne,ne,se,\
ne,ne,ne,ne,ne,ne,ne,ne,ne,ne,ne,s,ne,ne,n,ne,ne,ne,ne,n,ne,sw,ne,ne,ne,ne,s,se,ne,ne,n,n,ne,ne,ne,ne,sw\
,sw,ne,s,ne,ne,ne,ne,ne,n,n,n,ne,ne,ne,ne,ne,ne,ne,ne,ne,ne,ne,ne,ne,ne,ne,sw,sw,ne,n,ne,ne,n,ne,ne,ne,s\
w,ne,ne,ne,ne,n,ne,ne,ne,ne,n,ne,n,se,ne,s,nw,ne,sw,sw,n,n,n,ne,ne,n,ne,sw,ne,ne,n,ne,n,ne,ne,ne,ne,ne,s\
,sw,n,ne,ne,ne,se,ne,s,ne,ne,s,s,ne,ne,s,ne,n,ne,n,n,ne,n,ne,ne,ne,ne,ne,sw,ne,sw,ne,ne,ne,ne,ne,ne,ne,n\
e,ne,ne,ne,ne,ne,se,n,ne,ne,ne,s,ne,ne,ne,ne,ne,ne,ne,ne,sw,nw,ne,ne,nw,nw,n,ne,ne,sw,ne,nw,ne,ne,n,ne,n\
w,se,ne,ne,ne,ne,s,ne,ne,n,ne,ne,se,n,ne,n,ne,ne,ne,ne,ne,ne,ne,nw,ne,ne,sw,ne,n,n,sw,ne,ne,ne,ne,ne,ne,\
ne,ne,ne,ne,ne,s,nw,ne,ne,ne,ne,ne,ne,ne,nw,ne,ne,s,ne,ne,ne,ne,ne,n,ne,ne,ne,ne,ne,ne,s,ne,ne,ne,ne,ne,\
ne,s,ne,ne,ne,ne,ne,ne,ne,ne,ne,se,ne,ne,ne,nw,ne,ne,ne,n,nw,nw,sw,sw,sw,sw,nw,s,se,s,s,s,se,ne,se,n,se,\
s,ne,s,ne,nw,se,se,ne,se,ne,ne,s,se,se,ne,ne,ne,ne,ne,s,sw,ne,ne,n,ne,ne,ne,n,n,n,n,n,n,n,n,n,nw,n,n,n,n\
e,sw,sw,n,n,n,ne,n,s,se,nw,n,nw,nw,n,n,sw,nw,s,n,n,sw,sw,n,nw,nw,nw,n,se,nw,nw,sw,n,nw,n,nw,nw,nw,nw,nw,\
nw,nw,nw,ne,n,nw,n,nw,s,s,se,nw,s,se,sw,nw,s,nw,s,sw,nw,nw,nw,sw,nw,nw,nw,nw,sw,s,nw,ne,sw,ne,sw,sw,nw,s\
w,nw,sw,sw,nw,ne,sw,sw,ne,se,nw,sw,sw,sw,sw,sw,s,sw,s,sw,sw,s,sw,s,sw,sw,s,ne,se,s,sw,sw,sw,s,s,s,se,s,s\
w,sw,s,s,s,nw,s,s,s,s,s,n,s,sw,nw,sw,s,sw,sw,sw,s,s,s,sw,se,se,s,s,s,s,s,s,s,s,s,s,s,se,s,s,sw,s,s,s,se,\
s,se,s,s,s,s,s,s,se,se,se,s,s,s,nw,s,se,ne,se,s,se,s,se,s,s,s,s,se,ne,se,s,s,s,s,s,ne,s,s,s,se,s,s,nw,se\
,nw,se,s,se,se,s,se,s,n,sw,s,se,s,s,n,n,s,se,se,se,nw,se,sw,se,s,se,s,se,se,se,s,se,se,s,sw,sw,n,s,se,se\
,se,se,se,se,se,se,ne,se,se,s,se,se,se,se,se,se,nw,s,se,se,se,se,se,se,se,se,nw,se,se,se,se,se,se,se,s,n\
,se,se,se,se,se,se,ne,se,se,se,se,se,s,sw,se,se,se,s,se,se,se,se,ne,se,se,se,se,ne,se,ne,se,se,se,ne,ne,\
ne,se,se,ne,sw,se,se,ne,se,sw,ne,ne,se,se,ne,se,ne,se,nw,n,ne,se,se,ne,se,ne,se,se,ne,ne,s,se,nw,ne,se,s\
e,se,se,ne,ne,se,se,ne,ne,se,se,ne,s,ne,ne,se,se,ne,s,s,ne,ne,se,s,ne,se,ne,se,ne,ne,ne,ne,n,ne,ne,se,nw\
,se,ne,ne,ne,ne,ne,ne,nw,ne,ne,n,ne,ne,ne,ne,ne,nw,n,ne,ne,s,ne,ne,ne,ne,n,ne,ne,ne,s,ne,ne,ne,ne,ne,ne,\
ne,ne,ne,ne,ne,ne,ne,n,ne,ne,se,ne,ne,ne,n,sw,ne,nw,s,ne,n,ne,ne,ne,ne,ne,sw,ne,ne,ne,ne,ne,n,ne,ne,ne,n\
e,ne,ne,ne,sw,ne,ne,ne,s,n,ne,n,n,ne,ne,n,s,ne,n,ne,s,n,ne,ne,ne,ne,ne,ne,ne,n,ne,n,n,ne,s,ne,ne,n,ne,ne\
,ne,nw,s,n,se,ne,ne,n,n,ne,n,sw,se,ne,sw,ne,ne,ne,n,n,se,ne,s,n,n,n,ne,ne,n,n,nw,ne,ne,n,n,ne,ne,ne,n,n,\
ne,n,n,n,n,n,ne,ne,ne,n,n,n,ne,n,n,sw,n,n,ne,ne,s,ne,sw,ne,n,s,n,se,n,ne,n,n,n,n,ne,ne,n,n,n,sw,n,sw,n,n\
e,n,n,n,sw,n,n,n,s,ne,n,n,n,n,sw,n,sw,n,n,n,n,n,s,s,se,n,n,n,n,n,nw,s,s,n,n,n,n,n,n,nw,n,n,n,n,n,n,sw,se\
,n,n,n,n,n,nw,s,n,sw,sw,n,nw,n,ne,nw,s,n,n,n,s,nw,n,n,n,n,n,n,n,n,s,n,n,n,n,n,n,n,sw,n,n,n,n,n,n,ne,s,nw\
,sw,sw,ne,nw,n,n,n,n,nw,ne,n,n,nw,n,se,n,n,nw,nw,n,sw,nw,n,n,nw,n,n,nw,n,n,n,n,nw,nw,nw,n,nw,nw,n,n,nw,n\
,n,nw,n,sw,n,nw,n,n,n,s,n,nw,n,n,s,n,n,n,n,nw,n,n,s,nw,nw,n,sw,nw,n,n,n,n,nw,n,nw,n,n,n,s,ne,n,n,se,n,se\
,n,n,nw,se,n,nw,nw,nw,s,nw,n,nw,nw,n,nw,nw,se,n,nw,sw,nw,nw,nw,nw,n,nw,nw,nw,nw,sw,n,n,nw,n,s,se,n,n,n,n\
w,s,nw,n,nw,n,n,nw,nw,nw,n,nw,n,nw,n,n,s,n,nw,nw,n,ne,n,n,n,n,nw,s,nw,n,nw,nw,nw,nw,ne,nw,n,n,n,nw,nw,n,\
nw,nw,n,nw,nw,n,n,n,n,nw,nw,ne,ne,ne,nw,n,sw,nw,n,nw,nw,nw,nw,nw,nw,nw,nw,nw,nw,nw,s,nw,ne,nw,se,nw,nw,n\
w,ne,se,nw,nw,nw,nw,n,ne,nw,nw,nw,nw,nw,nw,nw,nw,nw,nw,ne,nw,nw,nw,nw,n,ne,nw,nw,nw,nw,nw,se,nw,nw,nw,nw\
,s,sw,nw,nw,se,nw,nw,ne,nw,nw,nw,nw,nw,sw,nw,sw,ne,nw,se,ne,nw,nw,sw,sw,ne,s,nw,se,nw,sw,nw,nw,nw,nw,nw,\
se,nw,nw,nw,sw,nw,nw,sw,ne,nw,sw,nw,nw,nw,s,s,nw,nw,nw,n,se,nw,nw,nw,nw,nw,nw,nw,sw,sw,ne,s,n,nw,nw,se,nw\
,nw,sw,nw,se,nw,nw,se,s,se,sw,sw,nw,nw,nw,n,nw,nw,nw,se,nw,sw,nw,nw,sw,nw,sw,nw,n,nw,sw,ne,se,nw,nw,s,sw,\
nw,n,sw,nw,sw,se,ne,sw,nw,nw,sw,s,nw,sw,nw,n,sw,nw,ne,ne,nw,s,nw,nw,nw,nw,sw,nw,sw,sw,nw,sw,nw,nw,s,nw,nw\
,nw,nw,se,nw,nw,sw,nw,n,nw,nw,nw,nw,sw,nw,nw,sw,nw,sw,sw,nw,sw,nw,nw,sw,n,sw,nw,sw,nw,nw,nw,sw,sw,sw,sw,nw\
,sw,sw,nw,nw,sw,s,sw,n,sw,nw,nw,sw,n,sw,sw,ne,nw,sw,s,sw,ne,nw,nw,n,sw,nw,sw,sw,s,n,sw,sw,nw,se,sw,nw,nw,n\
e,sw,nw,ne,sw,nw,sw,sw,nw,n,sw,ne,sw,n,s,sw,sw,sw,sw,sw,nw,sw,s,n,sw,nw,ne,sw,sw,sw,nw,se,se,nw,sw,se,sw,\
n,sw,sw,ne,sw,s,nw,sw,sw,sw,sw,se,nw,n,sw,sw,sw,sw,se,sw,nw,sw,nw,s,nw,sw,sw,ne,nw,sw,sw,sw,sw,sw,s,nw,sw\
,s,sw,nw,sw,nw,sw,se,sw,n,nw,sw,sw,sw,sw,nw,sw,sw,sw,nw,s,nw,sw,n,sw,n,sw,sw,sw,nw,sw,sw,sw,sw,sw,sw,sw,s\
w,s,nw,n,sw,sw,sw,sw,sw,n,nw,sw,s,sw,sw,sw,sw,se,nw,sw,sw,nw,sw,sw,sw,sw,sw,sw,nw,sw,sw,sw,se,sw,sw,sw,sw\
,sw,sw,sw,nw,s,sw,sw,sw,sw,nw,sw,sw,sw,n,sw,ne,nw,n,sw,sw,sw,sw,sw,sw,sw,sw,sw,se,sw,sw,se,sw,sw,nw,nw,sw\
,sw,ne,sw,sw,s,sw,nw,sw,sw,sw,ne,ne,sw,ne,sw,sw,sw,sw,sw,s,se,sw,s,nw,sw,sw,ne,sw,nw,sw,s,sw,sw,sw,s,sw,s\
w,n,se,sw,sw,sw,sw,sw,sw,s,sw,ne,sw,ne,s,sw,n,sw,s,n,sw,sw,sw,sw,nw,sw,sw,s,sw,ne,ne,sw,sw,sw,sw,sw,sw,se\
,sw,sw,sw,n,sw,sw,sw,sw,sw,sw,ne,sw,s,s,sw,sw,sw,sw,sw,se,sw,n,ne,sw,sw,nw,s,ne,sw,sw,sw,s,sw,sw,sw,sw,sw\
,sw,se,ne,se,se,sw,sw,nw,s,sw,sw,sw,sw,sw,ne,sw,sw,n,sw,se,s,sw,sw,n,sw,sw,se,sw,sw,sw,s,s,nw,ne,s,sw,sw,\
s,s,n,n,s,sw,s,sw,s,s,s,sw,se,s,s,n,sw,nw,sw,sw,sw,sw,sw,sw,sw,sw,s,sw,sw,s,ne,ne,sw,se,s,sw,s,sw,s,nw,sw\
,s,s,s,s,nw,sw,s,nw,sw,s,s,ne,s,s,sw,s,sw,se,sw,sw,s,sw,n,n,s,s,sw,sw,ne,ne,sw,s,s,sw,sw,s,s,sw,s,s,s,s,s\
,s,ne,sw,sw,sw,sw,s,sw,s,s,n,s,sw,sw,s,nw,s,sw,sw,s,sw,nw,sw,sw,sw,sw,n,s,sw,sw,ne,s,ne,s,sw,sw,sw,sw,s,s\
w,sw,s,s,s,sw,s,s,sw,s,s,s,sw,se,nw,se,s,sw,s,sw,s,se,sw,s,s,sw,s,sw,sw,s,sw,s,sw,sw,sw,sw,sw,s,sw,s,s,ne\
,sw,s,s,s,sw,s,s,s,s,s,s,s,n,s,nw,se,s,sw,s,sw,s,sw,sw,sw,ne,nw,sw,sw,s,s,sw,s,s,s,s,ne,se,s,s,s,sw,s,sw,\
sw,s,s,s,sw,s,s,s,s,s,sw,s,s,s,s,n,s,s,s,ne,s,s,sw,sw,s,s,s,ne,s,n,s,sw,s,sw,sw,s,s,sw,sw,sw,sw,sw,sw,s,n\
,n,s,ne,ne,sw,nw,s,s,s,nw,s,s,s,nw,ne,s,s,s,s,s,n,ne,s,s,sw,s,s,s,s,s,ne,sw,s,s,se,s,sw,n,n,s,sw,ne,ne,s,\
se,s,s,ne,s,s,s,sw,se,s,s,s,sw,s,s,s,nw,s,se,s,s,s,nw,sw,s,s,sw,s,s,s,s,s,sw,se,s,s,s,s,s,s,n,nw,s,s,se,n\
w,s,s,s,s,s,se,se,s,s,n,s,s,nw,nw,n,sw,se,s,s,n,s,s,s,s,sw,sw,nw,s,ne,s,n,s,nw,ne,s,s,s,s,s,s,s,s,s,sw,sw\
,s,s,s,s,sw,s,nw,n,s,s,s,s,s,s,s,s,s,s,sw,s,s,s,ne,n,nw,s,sw,s,s,s,s,s,nw,s,s,s,s,nw,s,s,n,s,s,s,s,nw,s,n\
,nw,s,n,s,s,s,nw,s,nw,s,s,s,s,sw,s,sw,s,s,s,s,s,s,sw,s,s,s,n,s,sw,s,s,s,n,ne,s,s,s,s,s,s,nw,s,s,s,nw,se,s\
,s,sw,s,s,s,sw,s,sw,se,n,s,sw,s,s,s,s,s,ne,se,s,nw,s,s,n,n,s,s,s,s,s,se,s,se,s,s,se,n,s,s,ne,s,s,s,s,s,s,\
s,s,s,s,s,se,s,s,s,s,s,se,s,s,se,n,sw,s,s,s,s,s,s,s,s,s,s,s,se,s,nw,s,s,n,s,nw,s,s,nw,s,s,n,s,sw,s,s,n,s,\
s,s,s,s,s,sw,s,s,nw,s,nw,se,se,se,s,s,n,se,s,s,n,s,s,n,se,s,se,nw,se,se,s,s,s,se,n,se,se,s,se,s,s,s,s,s,s\
,s,nw,se,s,s,nw,se,ne,se,ne,s,s,s,s,s,n,sw,se,se,s,sw,se,s,s,s,se,s,se,s,s,s,s,s,s,se,s,s,s,s,s,ne,se,se,\
s,s,se,se,s,se,s,s,s,nw,s,s,s,se,s,se,s,se,s,n,sw,ne,se,ne,s,se,s,s,s,s,s,s,s,s,s,s,n,s,n,s,se,ne,s,nw,s,\
s,se,nw,sw,se,se,s,se,s,s,sw,s,n,s,s,s,s,ne,se,s,s,ne,s,ne,se,se,s,ne,se,s,s,se,se,s,n,se,s,n,se,s,sw,se,\
s,se,s,s,se,s,s,s,nw,s,se,s,s,s,s,se,s,s,s,s,ne,s,s,s,s,se,ne,s,sw,se,s,s,sw,s,sw,s,sw,se,s,n,s,s,s,n,s,s\
e,se,s,s,s,s,s,se,se,se,s,se,ne,s,se,s,se,s,s,s,se,s,se,se,s,nw,s,n,s,s,s,s,s,se,se,s,ne,nw,n,se,ne,s,se,\
se,se,sw,se,ne,se,s,s,se,s,s,se,n,sw,nw,se,ne,s,se,s,s,se,se,s,s,s,n,s,s,n,s,se,s,nw,se,n,se,se,nw,se,s,s\
w,se,se,s,s,se,se,se,n,s,se,se,se,ne,se,sw,s,s,s,s,se,s,s,se,se,nw,nw,s,n,se,s,se,se,s,se,s,se,nw,ne,se,s\
e,s,nw,s,s,s,se,s,se,se,s,sw,se,se,s,se,se,se,ne,se,s,se,s,s,s,se,sw,se,s,se,nw,se,nw,se,n,n,s,s,s,se,se,\
se,ne,s,se,se,se,sw,s,se,nw,se,nw,se,se,s,se,ne,s,se,se,sw,s,se,se,nw,s,s,se,se,se,se,s,se,se,s,se,ne,se,\
n,sw,se,ne,se,se,se,se,se,n,s,se,se,s,s,s,sw,se,se,s,s,se,s,se,se,se,se,se,se,ne,s,s,s,se,se,se,s,s,se,se\
,se,ne,s,s,s,se,se,se,se,se,se,n,se,s,s,se,se,se,se,se,se,s,se,se,se,s,n,se,sw,s,se,sw,se,se,se,s,nw,se,s\
,se,se,se,se,s,se,s,se,sw,se,ne,se,s,se,sw,se,s,se,se,se,se,s,se,ne,se,nw,se,se,se,se,s,se,s,se,se,ne,se,\
s,se,nw,sw,se,ne,se,n,se,s,se,s,se,sw,se,se,s,se,se,se,se,se,se,se,sw,s,se,se,se,se,se,ne,se,se,nw,se,nw,\
se,nw,se,se,se,se,se,se,se,se,se,se,sw,sw,se,se,se,n,se,se,n,s,se,se,n,se,se,se,se,sw,nw,se,se,se,se,se,s\
e,se,se,se,s,se,se,s,se,se,se,s,se,s,sw,se,se,se,sw,se,n,se,sw,se,s,se,se,se,se,nw,se,se,se,se,se,se,se,s\
e,se,se,se,se"""

pset_input = pset_input.split(',')

hg = HexGrid()
print hg.get_steps_from_center(pset_input)