# Day 17

BUFFER_SIZE = 2017
SUPER_BUFFER_SIZE = 50000000

class Spinlock(object):
    buffer = [0]
    position = 0
    position_0 = 0

    # Regenerate the circular buffer from scratch and return the value after 2017 in the buffer
    def regenerate(self, num_steps):
        # reinitialize the buffer
        self.buffer = [0]
        self.position = 0

        # Use a while loop here so we don't have to build an extremely large list
        i = 1
        while i <= BUFFER_SIZE:
            # Move through the circular buffer (to the new jump position)
            jump_position = (self.position + num_steps) % len(self.buffer)

            # Insert value i into the list right after the element at hand (at jump position + 1)
            self.buffer.insert(jump_position + 1, i)


            # Set the new current position (jump position + 1)
            self.position = jump_position + 1

            i += 1

        return self.buffer[self.position + 1]

    def find_after_0(self, num_steps):
        self.position = 0
        num_after_0 = -1

        i = 1
        while i <= SUPER_BUFFER_SIZE:
            jump_position = (self.position + num_steps) % i

            if jump_position == 0:
                num_after_0 = i

            self.position = jump_position + 1

            i += 1

        return num_after_0




pset_input = 363

sl = Spinlock()

print sl.find_after_0(pset_input)
