# Day 10

HASH_SIZE = 256
REPEATS = 64

class KnotHash(object):
    hash = []

    def get_knot_hash(self, lengths):
        self.hash = []
        # Convert each character to an item in the list
        lengths = list(lengths)
        # Convert each character to the ASCII decimal value equivalent
        for i in range(0, len(lengths)):
            lengths[i] = ord(lengths[i])

        dense_hash = self.get_hash_by_lengths(lengths)

        hex_string = ''
        for item in dense_hash:
            hex_fragment = hex(item).split('0x')[1]
            for i in range(len(hex_fragment), 2):
                hex_fragment = '0' + hex_fragment
            hex_string += hex_fragment

        return hex_string

    def get_hash_by_lengths(self, lengths):
        current_position = 0
        skip_size = 0
        for h in range(0, HASH_SIZE):
            self.hash.append(h)

        # Append the standard required inclusion
        lengths += [17, 31, 73, 47, 23]

        lengths_full_run = lengths * REPEATS

        for length in lengths_full_run:
            if length != 0:
                # we're going to need to manually reverse since we need to accomodate for circular activity
                end_position = (current_position + length - 1) % HASH_SIZE
                self.reverse_circular(current_position, end_position)

            current_position = (current_position + length + skip_size) % HASH_SIZE
            skip_size += 1

        return self.sparse_to_dense_hash()

    def reverse_circular(self, start, end):
        tracking_start = start
        tracking_end = end
        if end < start:
            tracking_end += HASH_SIZE

        while tracking_end >= tracking_start:
            swapper = self.hash[start]
            self.hash[start] = self.hash[end]
            self.hash[end] = swapper

            start = (start + 1) % HASH_SIZE
            end = (end - 1) % HASH_SIZE
            tracking_start += 1
            tracking_end -= 1
        return

    def sparse_to_dense_hash(self):
        dense_hash = [0] * (HASH_SIZE / 16)
        for h in range(0, len(self.hash)):
            if h % 16 == 0:
                dense_hash[h / 16] = self.hash[h]
            else:
                dense_hash[h / 16] = dense_hash[h / 16] ^ self.hash[h]

        return dense_hash


