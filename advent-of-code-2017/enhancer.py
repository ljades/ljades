# Day 21

ON = '#'
OFF = '.'

def fresh_square(grid_size):
    grid = []
    grid_y = 0
    while grid_y < grid_size:
        grid.append([OFF] * grid_size)

        grid_y += 1

    return grid

def pretty_print(grid):
    for line in grid:
        print ''.join(line)

# This class is used for placing a x-by-x in its "normalized" transformation, for easier comparison
class Normalizer(object):

    def get_normalized_grid(self, grid_input):
        max_grid = grid_input
        temp_grid = fresh_square(len(grid_input))

        max_grid_val = self.grid_val(grid_input)

        is_transformed = False

        # Structure: Highest grid value is the determinant for the best unique transformation
        #               for a grid. Find the best grid value from the following transformations:
        #           Original, Rotate90, Rotate180, Rotate270, FlipX, FlipY
        #           If Original is picked, return that as the "normalized" grid.
        #           Else, return the function called recursively, with the new picked transformation as the "normalized" grid
        #   ALWAYS RETURN GRID COPIES, NEVER THE ORIGINAL

        # TODO: Make the following comparisons more modular
        # r90
        temp_grid = self.get_r90(grid_input, temp_grid)
        temp_grid_val = self.grid_val(temp_grid)
        if temp_grid_val > max_grid_val:
            max_grid_val = temp_grid_val
            max_grid = self.get_clone(temp_grid)
            is_transformed = True

        # r180
        temp_grid = self.get_r180(grid_input, temp_grid)
        temp_grid_val = self.grid_val(temp_grid)
        if temp_grid_val > max_grid_val:
            max_grid_val = temp_grid_val
            max_grid = self.get_clone(temp_grid)
            is_transformed = True
        
        # r270
        temp_grid = self.get_r270(grid_input, temp_grid)
        temp_grid_val = self.grid_val(temp_grid)
        if temp_grid_val > max_grid_val:
            max_grid_val = temp_grid_val
            max_grid = self.get_clone(temp_grid)
            is_transformed = True
        
        # flipx
        temp_grid = self.get_flipx(grid_input, temp_grid)
        temp_grid_val = self.grid_val(temp_grid)
        if temp_grid_val > max_grid_val:
            max_grid_val = temp_grid_val
            max_grid = self.get_clone(temp_grid)
            is_transformed = True

        # flipy
        temp_grid = self.get_flipy(grid_input, temp_grid)
        temp_grid_val = self.grid_val(temp_grid)
        if temp_grid_val > max_grid_val:
            max_grid_val = temp_grid_val
            max_grid = self.get_clone(temp_grid)
            is_transformed = True

        temp_grid2 = fresh_square(len(grid_input))
        # r90_flipx
        temp_grid = self.get_r90(grid_input, temp_grid)
        temp_grid2 = self.get_flipx(temp_grid, temp_grid2)
        temp_grid_val = self.grid_val(temp_grid2)
        if temp_grid_val > max_grid_val:
            max_grid_val = temp_grid_val
            max_grid = self.get_clone(temp_grid2)
            is_transformed = True

        # r270_flipx
        temp_grid = self.get_r270(grid_input, temp_grid)
        temp_grid2 = self.get_flipx(temp_grid, temp_grid2)
        temp_grid_val = self.grid_val(temp_grid2)
        if temp_grid_val > max_grid_val:
            max_grid_val = temp_grid_val
            max_grid = self.get_clone(temp_grid2)
            is_transformed = True

        # r90_flipy
        temp_grid = self.get_r90(grid_input, temp_grid)
        temp_grid2 = self.get_flipy(temp_grid, temp_grid2)
        temp_grid_val = self.grid_val(temp_grid2)
        if temp_grid_val > max_grid_val:
            max_grid_val = temp_grid_val
            max_grid = self.get_clone(temp_grid2)
            is_transformed = True

        # r270_flipy
        temp_grid = self.get_r270(grid_input, temp_grid)
        temp_grid2 = self.get_flipy(temp_grid, temp_grid2)
        temp_grid_val = self.grid_val(temp_grid2)
        if temp_grid_val > max_grid_val:
            max_grid_val = temp_grid_val
            max_grid = self.get_clone(temp_grid2)
            is_transformed = True

        if is_transformed:
            return self.get_normalized_grid(max_grid)

        return grid_input

    def get_clone(self, grid_input):
        grid_output = fresh_square(len(grid_input))
        size = len(grid_input)
        for y in range(0, size):
            for x in range(0, size):
                grid_output[y][x] = grid_input[y][x]

        return grid_output

    def get_r90(self, grid_input, grid_output):
        size = len(grid_input)
        for y in range(0, size):
            for x in range(0, size):
                grid_output[x][size - y - 1] = grid_input[y][x]

        return grid_output

    def get_r180(self, grid_input, grid_output):
        size = len(grid_input)
        for y in range(0, size):
            for x in range(0, size):
                grid_output[size - y - 1][size - x - 1] = grid_input[y][x]

        return grid_output

    def get_r270(self, grid_input, grid_output):
        size = len(grid_input)
        for y in range(0, size):
            for x in range(0, size):
                grid_output[size - x - 1][y] = grid_input[y][x]

        return grid_output

    def get_flipx(self, grid_input, grid_output):
        size = len(grid_input)
        for y in range(0, size):
            for x in range(0, size):
                grid_output[size - y - 1][x] = grid_input[y][x]

        return grid_output

    def get_flipy(self, grid_input, grid_output):
        size = len(grid_input)
        for y in range(0, size):
            for x in range(0, size):
                grid_output[y][size - x - 1] = grid_input[y][x]

        return grid_output

    def cell_val(self, cell, position):
        if cell == ON:
            return 2**position
        return 0

    # Grid value is determined by equating the grid to a bit string, where the LSB is the leftmost bit.
    # The bit string is interpreted by reading the cells of the grid, going down the row for each row.
    def grid_val(self, grid):
        total_val = 0

        curr_position = 0
        for row in grid:
            for col_cell in row:
                total_val += self.cell_val(col_cell, curr_position)
                curr_position += 1

        return total_val


# This class is the primary one involved in this code problem--used for taking a grid and enhancing it
# any number of iterations
class Enhancer(object):
    full_art = []
    rules_optimized = {} # For Part 2, this is also continuously memoizes non-normalized keystrings
    normalizer = Normalizer()

    def __init__(self, initial_art_raw):
        initial_art_split = initial_art_raw.split('\n')
        total_size = len(initial_art_split)
        self.full_art = fresh_square(total_size)

        for y in range(0, total_size):
            for x in range(0, total_size):
                self.full_art[y][x] = initial_art_split[y][x]


    def count_on(self):
        total_count = 0

        for row in self.full_art:
            for col_cell in row:
                if col_cell == ON:
                    total_count += 1
        
        return total_count

    def enhance_and_count(self, rules_raw, total_iterations):
        self.parse_and_optimize_rules(rules_raw)
        prev_count = 1

        # For loop without generating a new array, for efficiency purposes on large
        # iteration config
        curr_iter = 0
        while curr_iter < total_iterations:
            print self.count_on(), (float(self.count_on()) / float(prev_count))
            prev_count = self.count_on()
            print ''

            self.enhance()

            curr_iter += 1

        return self.count_on()

    def parse_and_optimize_rules(self, rules_raw):
        rules_split = rules_raw.split('\n')
        for r_raw in rules_split:
            before_key, after_key = r_raw.split(' => ')
            before_key_normalized = self.grid_to_key(
                self.normalizer.get_normalized_grid(
                    self.key_to_grid(before_key)))
            self.rules_optimized[before_key_normalized] = after_key

        return

    def key_to_grid(self, key):
        key_split = key.split('/')
        size = len(key_split)
        grid = fresh_square(size)

        for y in range(0, size):
            for x in range(0, size):
                grid[y][x] = key_split[y][x]

        return grid

    def grid_to_key(self, grid):
        grid_strings = []
        for line in grid:
            grid_strings.append(''.join(line))


        return '/'.join(grid_strings)

    def enhance(self):

        # Determine individual grid size, and number of grids in each dimension (equal)
        if len(self.full_art) % 2 == 0:
            grid_size = 2
        else:
            grid_size = 3

        # Extract every individual grid
        grid_list = []

        num_grids_in_row = len(self.full_art) / grid_size
        num_grids = num_grids_in_row ** 2
        curr_grid = 0
        # Use while loop to avoid creating a full range list
        while curr_grid < num_grids:
            start_pos = ((curr_grid / num_grids_in_row) * grid_size, (curr_grid % num_grids_in_row) * grid_size)
            grid_list.append(self.extract_grid_from_art(start_pos, grid_size))

            curr_grid += 1

        enhanced_grid_list = []
        # For each individual grid:
        for old_grid in grid_list:
            # Check if the key, unnormalized, is memoized
            if self.grid_to_key(old_grid) in self.rules_optimized:
                enhanced_grid_key = self.rules_optimized[self.grid_to_key(old_grid)]
            else:
                #   - Normalize
                old_grid_normalized = self.normalizer.get_normalized_grid(old_grid)
                #   - Convert from grid to key
                old_grid_key = self.grid_to_key(old_grid_normalized)
                #   - Query the key in the rules
                if old_grid_key in self.rules_optimized:
                    enhanced_grid_key = self.rules_optimized[old_grid_key]
                    # Memo the unnormalized key
                    self.rules_optimized[self.grid_to_key(old_grid)] = enhanced_grid_key
                else:
                    #       - If nothing comes up, there's a problem with the rules
                    print "No key found. Something went wrong."
                    exit()

            #   - Convert the returned value from a key to grid
            #   - Append to the new grid list
            enhanced_grid_list.append(self.key_to_grid(enhanced_grid_key))

        if grid_size == 2:
            enhanced_grid_size = 3
        else:
            enhanced_grid_size = 4

        #   - Replace full_art with new blank slate of the new size
        self.full_art = fresh_square(num_grids_in_row * enhanced_grid_size)
        
        #   - Apply every individual grid to the new full_art
        curr_grid = 0
        while curr_grid < num_grids:
            self.apply_grid_to_art(enhanced_grid_list[curr_grid], curr_grid, enhanced_grid_size, num_grids_in_row)

            curr_grid += 1

        return

    # start_pos is a tuple in the format (y_position, x_position)
    def extract_grid_from_art(self, start_pos, grid_size):
        grid = fresh_square(grid_size)

        for y in range(0, grid_size):
            for x in range(0, grid_size):
                grid[y][x] = self.full_art[start_pos[0] + y][start_pos[1] + x]
        return grid

    def apply_grid_to_art(self, grid, grid_index, grid_size, num_grids_in_row):
        start_pos = ((grid_index / num_grids_in_row) * grid_size, (grid_index % num_grids_in_row) * grid_size)
        for y in range(0, grid_size):
            for x in range(0, grid_size):
                self.full_art[start_pos[0] + y][start_pos[1] + x] = grid[y][x]

        return



starting_art = """.#.
..#
###"""

pset_input = """../.. => .../.../..#
#./.. => #.#/..#/...
##/.. => #.#/..#/#.#
.#/#. => #../.../.##
##/#. => ###/#.#/..#
##/## => #.#/.../#..
.../.../... => #..#/..../.##./....
#../.../... => ..../.##./#.../.##.
.#./.../... => .#../####/..##/#...
##./.../... => ##.#/..#./####/...#
#.#/.../... => ##.#/##../#.#./.#..
###/.../... => #..#/#..#/##../##.#
.#./#../... => #.##/##../.#.#/..##
##./#../... => #.#./..../.###/.#.#
..#/#../... => ..##/####/..##/....
#.#/#../... => ..##/###./..##/#...
.##/#../... => #.../####/#..#/##..
###/#../... => ...#/..../..##/#...
.../.#./... => ##../##../..##/....
#../.#./... => #.../.#.#/.##./#..#
.#./.#./... => ..##/#.../...#/###.
##./.#./... => ####/.#.#/..##/####
#.#/.#./... => ####/.#../#.##/#..#
###/.#./... => ..#./#..#/.#.#/###.
.#./##./... => ##../.#.#/#..#/#..#
##./##./... => .###/####/#..#/..##
..#/##./... => ###./.#../..#./#.##
#.#/##./... => ##../#.#./#.../.#.#
.##/##./... => #.../#.../.#.#/####
###/##./... => .#../####/#.../#.#.
.../#.#/... => .#../..../##../.###
#../#.#/... => .##./...#/.###/...#
.#./#.#/... => ...#/#.../...#/####
##./#.#/... => #.##/..#./#..#/.#.#
#.#/#.#/... => #..#/..../..##/..#.
###/#.#/... => .#.#/#.#./##.#/#.#.
.../###/... => ##../.##./###./###.
#../###/... => ###./..##/.#../##.#
.#./###/... => .#../##../..../..##
##./###/... => #.#./...#/...#/##..
#.#/###/... => ..../.#../#.../.#..
###/###/... => ..#./.###/..../##.#
..#/.../#.. => #.#./.#../...#/##.#
#.#/.../#.. => ...#/##.#/#.#./#...
.##/.../#.. => ...#/..##/#.##/##.#
###/.../#.. => #..#/.#.#/.##./..#.
.##/#../#.. => ##../..#./#.##/##..
###/#../#.. => ..../###./#.#./##..
..#/.#./#.. => #.#./.##./.##./#...
#.#/.#./#.. => .#../#..#/#.#./#...
.##/.#./#.. => .#.#/#..#/..#./....
###/.#./#.. => #.##/####/#.../..#.
.##/##./#.. => #.##/.#.#/..../.#..
###/##./#.. => #.##/####/.###/##..
#../..#/#.. => ###./#.##/..#./..##
.#./..#/#.. => ##../.#../..#./..##
##./..#/#.. => #..#/.#../..../##.#
#.#/..#/#.. => .###/.##./..#./#.#.
.##/..#/#.. => .#.#/..../####/.#..
###/..#/#.. => .##./##../...#/.#..
#../#.#/#.. => #.#./#.##/..../.###
.#./#.#/#.. => ####/#.#./.#../#.##
##./#.#/#.. => ..##/.###/###./..#.
..#/#.#/#.. => .##./..#./..../#.#.
#.#/#.#/#.. => .###/..../..../##..
.##/#.#/#.. => #.#./#.../####/.###
###/#.#/#.. => #.../..##/###./#..#
#../.##/#.. => ..../#.#./..##/.#.#
.#./.##/#.. => ..##/..##/#..#/###.
##./.##/#.. => #.../.#../#.#./#.##
#.#/.##/#.. => ...#/#.../...#/###.
.##/.##/#.. => ###./..../..##/#..#
###/.##/#.. => #.#./##.#/####/#.#.
#../###/#.. => ##../##../###./#..#
.#./###/#.. => #.##/###./####/..##
##./###/#.. => ..../.###/###./.#..
..#/###/#.. => .###/..../..#./....
#.#/###/#.. => ####/#..#/.#.#/..##
.##/###/#.. => ..../##.#/####/##.#
###/###/#.. => #..#/.#.#/###./.##.
.#./#.#/.#. => #.##/...#/###./....
##./#.#/.#. => #..#/.#../..../#.#.
#.#/#.#/.#. => .#.#/####/..../.#.#
###/#.#/.#. => #.#./#.##/##.#/##..
.#./###/.#. => ..#./..../##../####
##./###/.#. => #.##/##.#/#.##/.#..
#.#/###/.#. => .#.#/..##/##.#/####
###/###/.#. => .#../...#/#..#/#.#.
#.#/..#/##. => .##./..#./...#/##.#
###/..#/##. => ..#./##.#/#..#/#..#
.##/#.#/##. => ##.#/#.../#..#/...#
###/#.#/##. => ##../.#../..../.##.
#.#/.##/##. => #.##/##.#/.#../.###
###/.##/##. => ..../#.#./##../##.#
.##/###/##. => ###./.#.#/.##./.###
###/###/##. => #..#/.###/#.../#...
#.#/.../#.# => .###/#.##/.#.#/#.#.
###/.../#.# => ...#/##../...#/##.#
###/#../#.# => ..../..#./..#./####
#.#/.#./#.# => ##../#.##/...#/#...
###/.#./#.# => #.#./...#/.#../#...
###/##./#.# => .#../..#./...#/##..
#.#/#.#/#.# => ####/#.##/.#../##..
###/#.#/#.# => #.../#.../###./.#..
#.#/###/#.# => ####/.#.#/.##./.#.#
###/###/#.# => #.##/.#.#/##.#/..##
###/#.#/### => .###/#.##/..../..#.
###/###/### => .###/#..#/##../.##."""

en = Enhancer(starting_art)
norm = Normalizer()

print en.enhance_and_count(pset_input, 18)
