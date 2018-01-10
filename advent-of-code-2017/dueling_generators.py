# Day 15

GENERATOR_RANGE = 2147483647

class Generator(object):
    value = 0
    factor = 0
    multiple = 0

    def __init__(self, factor, multiple, seed):
        self.factor = factor
        self.value = seed
        self.multiple = multiple

    def generate(self):
        self.value = (self.value * self.factor) % GENERATOR_RANGE
        while self.value % self.multiple != 0:
            self.value = (self.value * self.factor) % GENERATOR_RANGE
        return self.value


class Judge(object):
    factor_a = 16807
    factor_b = 48271
    multiple_a = 4
    multiple_b = 8
    validated_bits = 16
    validated_remainder = 2 ** validated_bits

    def judge(self, repetitions, seed_a, seed_b):
        num_matched = 0
        # Create the generators
        gen_a = Generator(self.factor_a, self.multiple_a, seed_a)
        gen_b = Generator(self.factor_b, self.multiple_b, seed_b)

        # For number of repetitions
        for _ in range(0, repetitions):
            # generate with both generators
            curr_a = gen_a.generate()
            curr_b = gen_b.generate()

            # validate results for a match
            if (curr_a % self.validated_remainder) == (curr_b % self.validated_remainder):
                num_matched += 1

        # Return number matched
        return num_matched


repetitions = 5000000
seed_a = 703
seed_b = 516
j = Judge()
print j.judge(repetitions, seed_a, seed_b)