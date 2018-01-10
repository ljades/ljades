# Day 3

import math

SECOND_TO_LAST = 265149

class SpiralMemory(object):

    def manhattan_distance(self, root, destination):
        dist = 0
        if root == 1:
            if destination == 1:
                return 0

            # Calculate square bound n
            n = self.calculate_square_bound(destination)

            # Find the median of the layer (not needed)
            # n_layer_median = (n ** 2) - (n - 1)

            # Find the medians of the two layer segments
            n_segment_medians = [(n ** 2) - ((n - 1) / 2),
                                    ((n - 1) ** 2) + ((n - 1) / 2)]

            dist = (n / 2) + min(abs(destination - n_segment_medians[0]), abs(destination - n_segment_medians[1]))


        return dist

    def calculate_square_bound(self, num):
        return int(math.ceil(math.sqrt(num)))


    def manhattan_coords(self, destination):
        dist = 0
        if destination == 1:
            return [0,0]

        # Calculate square bound n
        n = self.calculate_square_bound(destination)

        # Find the median of the layer (not needed)
        n_layer_median = (n ** 2) - (n - 1)

        delta_x = 0
        delta_y = 0
        if n % 2:
            n_layer_median_coords = [-1 * (n / 2), -1 * (n / 2)]

            if destination > n_layer_median:
                delta_x = destination - n_layer_median
            elif destination < n_layer_median:
                delta_y = n_layer_median - destination
        else:
            n_layer_median_coords = [n / 2, n / 2]

            if destination > n_layer_median:
                delta_x = n_layer_median - destination
            elif destination < n_layer_median:
                delta_y = destination - n_layer_median

        return [n_layer_median_coords[0] + delta_x, n_layer_median_coords[1] + delta_y]

    def destination_from_coords(self, coords):
        if coords[0] == 0 and coords[1] == 0:
            return 1

        max_coord = abs(max(coords[0], coords[1]))
        # Are we in an odd or even direction?
        if coords[0] + coords[1] > 0:
            max_coord = max(coords[0], coords[1])
            # Even
            n = 2 * max_coord

            n_layer_median = (n ** 2) - (n - 1)

            if coords[0] < max_coord:
                destination = n_layer_median + (max_coord - coords[0])
            elif coords[1] <= max_coord:
                destination = n_layer_median - (max_coord - coords[1])
        else:
            max_coord = min(coords[0], coords[1])
            # Odd
            n = (-2 * max_coord) + 1
            
            n_layer_median = (n ** 2) - (n - 1)

            if coords[0] > max_coord:
                destination = n_layer_median + (coords[0] - max_coord)
            elif coords[1] >= max_coord:
                destination = n_layer_median - (coords[1] - max_coord)
        
        return destination


    spiral = []

    def neighbor_spiral_fill(self, limit):
        # initialize the center of the spiral, which is stored as as list
        self.spiral = [1]

        while self.spiral[-1] <= limit:
            # new value in the spiral
            self.spiral.append(0)

            self.spiral[-1] += self.sum_of_neighbors(len(self.spiral) - 1)

        return self.spiral[-1]

    def sum_of_neighbors(self, ind):
        neighbor_sum = 0

        ind_coords = self.manhattan_coords(ind + 1)
        # Get neighbor indices, added in counter-clockwise order (order doesn't matter)
        neighbors = [[ind_coords[0] + 1, ind_coords[1]]]
        neighbors.append([ind_coords[0] + 1, ind_coords[1] + 1])
        neighbors.append([ind_coords[0], ind_coords[1] + 1])
        neighbors.append([ind_coords[0] - 1, ind_coords[1] + 1])
        neighbors.append([ind_coords[0] - 1, ind_coords[1]])
        neighbors.append([ind_coords[0] - 1, ind_coords[1] - 1])
        neighbors.append([ind_coords[0], ind_coords[1] - 1])
        neighbors.append([ind_coords[0] + 1, ind_coords[1] - 1])

        for neighbor in neighbors:
            neighbor_ind = self.destination_from_coords(neighbor) - 1
            if neighbor_ind < ind:
                neighbor_sum += self.spiral[neighbor_ind]

        return neighbor_sum



sm = SpiralMemory()

print sm.neighbor_spiral_fill(265149)
