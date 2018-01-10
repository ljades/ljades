# Day 13

DEPTH = 0
RANGE = 1

class FirewallScanners(object):

    # Every scanner has a period of ((range * 2) - 2)
    # If it's within the forward-moving pass, finding its position is simply the modulus of the period
    # If it's greater or equal to the range, then it's in the reverse-moving pass and its position is (2 * range) - period_pos - 2
    def get_scanner_pos(self, time, range):
        
        period_pos = time % ((range * 2) - 2)
        
        if period_pos < range:
            return period_pos
        else:
            return (2 * range) - period_pos - 2

    def get_delay(self, packet_pos, firewall):
        severity = 0
        delay = -1
        not_caught = False

        while not_caught == False:
            not_caught = True
            delay += 1
            for layer in firewall:
                if packet_pos == self.get_scanner_pos(layer[DEPTH] + delay, layer[RANGE]):
                    not_caught = False
                    break
        
        return delay



pset_input = pset_input.split('\n')
for p in range(0, len(pset_input)):
    string_split = pset_input[p].split(': ')
    pset_input[p] = [int(string_split[0]), int(string_split[1])]


pset_input = """0: 3
1: 2
2: 4
4: 4
6: 5
8: 6
10: 6
12: 6
14: 6
16: 8
18: 8
20: 8
22: 8
24: 10
26: 8
28: 8
30: 12
32: 14
34: 12
36: 10
38: 12
40: 12
42: 9
44: 12
46: 12
48: 12
50: 12
52: 14
54: 14
56: 14
58: 12
60: 14
62: 14
64: 12
66: 14
70: 14
72: 14
74: 14
76: 14
80: 18
88: 20
90: 14
98: 17"""

fs = FirewallScanners()

print fs.get_delay(0, pset_input)