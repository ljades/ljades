# Day 14

from knot_hash import KnotHash

DISK_SIZE = 128

class Defragmenter(object):
    knot_hash_generator = KnotHash()
    disk = []
    used_space = 0

    def generate_disk(self, key):
        self.disk = []
        self.used_space = 0
        knot_hashes = []

        # generate the knot hashes
        for i in range(0, DISK_SIZE):
            curr_input = key + '-' + str(i)

            knot_hashes.append(self.knot_hash_generator.get_knot_hash(curr_input))
        

        # generate the bit strings, while counting used space
        for knot_hash in knot_hashes:
            curr_bit_string = ''

            for hex_letter in knot_hash:
                bit_segment = bin(int(hex_letter, 16)).split('0b')[1]
                for i in range(len(bit_segment), 4):
                    bit_segment = '0' + bit_segment
                # add to the bit string
                curr_bit_string += bit_segment

                # count used bits
                self.used_space += self.get_used_bits_in_segment(bit_segment)

            self.disk.append(curr_bit_string)
                

        # return the disk and used space count
        return self.disk, self.used_space

    def get_used_bits_in_segment(self, bit_segment):
        used_count = 0
        for b in bit_segment:
            if b == '1':
                used_count += 1

        return used_count

    # depth first search across the grid for connected components
    def get_num_regions(self):
        num_regions = 0
        touched_grid = []
        for _ in range(0, DISK_SIZE):
            touched_grid.append([False] * DISK_SIZE)

        # for every space on the disk, treat it like a vertex with edges up, down, left and right
        # if the vertex is not touched, add it to a vertex stack and do a deep search, and
        # add 1 to the number of regions
        for i in range(0, len(touched_grid)):

            for j in range(0, len(touched_grid[i])):

                if touched_grid[i][j] == False and self.disk[i][j] == '1':
                    num_regions += 1

                    # Run the depth first search on the vertex
                    vertex_stack = [[i, j]]

                    while len(vertex_stack) > 0:
                        curr_v = vertex_stack.pop()
                        touched_grid[curr_v[0]][curr_v[1]] = True
                        
                        neighbors = [
                            [curr_v[0], curr_v[1] - 1],
                            [curr_v[0], curr_v[1] + 1],
                            [curr_v[0] - 1, curr_v[1]],
                            [curr_v[0] + 1, curr_v[1]]]
                        # Check edges for being touched and used and in range
                        for neighbor in neighbors:
                            if (self.is_vertex_in_range(neighbor) and not touched_grid[neighbor[0]][neighbor[1]] and 
                                self.disk[neighbor[0]][neighbor[1]] == '1'):
                                vertex_stack.append(neighbor)

                touched_grid[i][j] = True

        return num_regions



    def is_vertex_in_range(self, vertex):
        return vertex[0] >= 0 and vertex[0] < DISK_SIZE and vertex[1] >= 0 and vertex[1] < DISK_SIZE

        


pset_input = 'hfdlxzhv'

df = Defragmenter()

my_disk, my_used_space = df.generate_disk(pset_input)

for line in my_disk:
    print line
print my_used_space

print df.get_num_regions()
