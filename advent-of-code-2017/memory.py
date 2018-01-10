# Day 6

class MemoryBlocks(object):

    def num_redistribution_cycles(self, banks):
        banks_dict = {}
        num_redists = 0

        while not self.banks_to_key(banks) in banks_dict:
            banks_dict[self.banks_to_key(banks)] = num_redists
            banks = self.reallocate(banks)
            num_redists += 1

        return num_redists - banks_dict[self.banks_to_key(banks)]

    def banks_to_key(self, banks):
        banks_key = ''
        for bank in banks:
            banks_key += str(bank) + '.'

        return banks_key

    def reallocate(self, banks):
        # find the max index bank
        max_i = 0
        max_blocks = 0
        for i in range(0, len(banks)):
            if banks[i] > max_blocks:
                max_blocks = banks[i]
                max_i = i

        # store that index's bank's block number, set that index's bank to zero
        temp_store = banks[max_i]
        banks[max_i] = 0

        i = max_i
        # loop and distribute while that block number is greater than 0
        while temp_store > 0:
            i = (i + 1) % len(banks)
            temp_store -= 1
            banks[i] += 1

        return banks


pset_input = """0	5	10	0	11	14	13	4	11	8	8	7	1	4	12	11"""

pset_input = pset_input.split('\t')
for p in range(0, len(pset_input)):
    pset_input[p] = int(pset_input[p])

mb = MemoryBlocks()
print mb.num_redistribution_cycles(pset_input)
