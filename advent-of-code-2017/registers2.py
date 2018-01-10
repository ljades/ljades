# Day 18

from collections import deque

class RegisterStore(object):
    registers = {}
    has_sounded = False
    last_sound_freq = -1
    has_recovered = False
    last_recovered_freq = -1

    instructions = []
    instruction_buffer = 0

    total_sent = 0

    sent_queue = deque([])

    paired_register_store = ''

    # Instruction operators
    inst_ops = {}
    
    def __init__(self, instructions_raw, start_p):
        self.inst_ops = {
            'snd': self.send,
            'set': self.set_register,
            'add': self.add_to_register,
            'mul': self.mul_to_register,
            'mod': self.mod_to_register,
            'rcv': self.receive_to_register,
            'jgz': self.jump_gz,
        }
        
        self.instructions = self.parse_instructions(instructions_raw)
        self.sent_queue = deque([])
        self.registers = {}
        self.registers['p'] = start_p

    def set_paired_register_store(self, paired_register_store):
        self.paired_register_store = paired_register_store

    #
    # Instruction operators
    #
    def play_sound(self, x):
        self.has_sounded = True
        self.last_sound_freq = self.get_val(x)
        return True

    def set_register(self, x, y):
        self.set_register_val(x, self.get_val(y))
        return True

    def add_to_register(self, x, y):
        self.set_register_val(x, self.get_val(x) + self.get_val(y))
        return True

    def mul_to_register(self, x, y):
        self.set_register_val(x, self.get_val(x) * self.get_val(y))
        return True

    def mod_to_register(self, x, y):
        self.set_register_val(x, self.get_val(x) % self.get_val(y))
        return True

    def recover_nz(self, x):
        if self.get_val(x) != 0 and self.has_sounded:
            self.has_recovered = True
            self.last_recovered_freq = self.last_sound_freq
        return True
        
    # Jump by y offset. The addition to the instruction buffer here does not
    # include the invariant increment after every instruction
    def jump_gz(self, x, y):
        if self.get_val(x) > 0:
            if self.get_val(y) == 0:
                print 'Infinite loop detected'
                exit()
            self.instruction_buffer += (self.get_val(y) - 1)
        return True

    def send(self, x):
        self.total_sent += 1
        self.sent_queue.append(self.get_val(x))
        return True

    def receive_to_register(self, x):
        received_val = self.paired_register_store.receive_from_queue()
        if received_val == '':
            return False
        
        self.set_register_val(x, received_val)
        return True


    def receive_from_queue(self):
        if len(self.sent_queue) == 0:
            return ''
        else:
            return self.sent_queue.popleft()

    #
    # Black box abstraction functions for dealing with the registers
    #
    def set_register_val(self, reg, val):
        if not reg.isalpha():
            return
        else:
            self.registers[reg] = val

    def get_val(self, reg_or_val):
        if not reg_or_val.isalpha():
            return int(reg_or_val)
        else:
            if reg_or_val in self.registers:
                return self.registers[reg_or_val]
            else:
                self.registers[reg_or_val] = 0
                return 0

    # Deprecated from Part 1
    # Main function for Part 1: Get first recovered frequency
    def get_first_recovered_frequency(self, instructions_raw):
        # Instructions should be in the form of: a list of tuples, of the format (instruction, input_x, input_y).
        # If there is no input_y, it will be set to ''. There is always at least an input_x according to contract
        instructions = self.parse_instructions(instructions_raw)

        self.instruction_buffer = 0
        while (self.instruction_buffer < len(instructions)):
            curr_inst = instructions[self.instruction_buffer]

            if curr_inst[2] == '':
                self.inst_ops[curr_inst[0]](curr_inst[1])
            else:
                self.inst_ops[curr_inst[0]](curr_inst[1], curr_inst[2])

            if curr_inst[0] == 'rcv':
                break

            self.instruction_buffer += 1

        return self.last_recovered_freq

    def run_single(self):
        if self.instruction_buffer >= len(self.instructions):
            print 'no! why is it ' + str(self.instruction_buffer)
            self.instruction_buffer = 0
            return False

        curr_inst = self.instructions[self.instruction_buffer]

        if curr_inst[2] == '':
            exit_val = self.inst_ops[curr_inst[0]](curr_inst[1])
        else:
            exit_val = self.inst_ops[curr_inst[0]](curr_inst[1], curr_inst[2])

        if exit_val:
            self.instruction_buffer += 1
        
        return exit_val


    def parse_instructions(self, instructions_raw):
        instructions = []

        instructions_raw_split = instructions_raw.split('\n')

        for inst_raw in instructions_raw_split:
            inst_split = inst_raw.split(' ')
            if len(inst_split) == 2:
                instructions.append((inst_split[0], inst_split[1], ''))
            else:
                instructions.append((inst_split[0], inst_split[1], inst_split[2]))

        return instructions
