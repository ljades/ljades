# Day 18

from registers2 import RegisterStore

class Duet(object):

    program_0 = ''
    program_1 = ''

    def __init__(self, instructions_raw):
        self.program_0 = RegisterStore(instructions_raw, 0)
        print self.program_0
        print self.program_0.registers
        self.program_1 = RegisterStore(instructions_raw, 1)
        print self.program_0.registers
        print self.program_1.registers

        self.program_0.set_paired_register_store(self.program_1)
        self.program_1.set_paired_register_store(self.program_0)

    def perform_duet(self):
        is_deadlock = False

        while not is_deadlock:
            exit_val_0 = self.program_0.run_single()
            exit_val_1 = self.program_1.run_single()

            if not exit_val_0 or not exit_val_1:
                print [exit_val_0, exit_val_1]

            if (not exit_val_0) and (not exit_val_1):
                is_deadlock = True
        
        return self.program_1.total_sent



pset_input = """set i 31
set a 1
mul p 17
jgz p p
mul a 2
add i -1
jgz i -2
add a -1
set i 127
set p 316
mul p 8505
mod p a
mul p 129749
add p 12345
mod p a
set b p
mod b 10000
snd b
add i -1
jgz i -9
jgz a 3
rcv b
jgz b -1
set f 0
set i 126
rcv a
rcv b
set p a
mul p -1
add p b
jgz p 4
snd a
set a b
jgz 1 3
snd b
set f 1
add i -1
jgz i -11
snd a
jgz f -16
jgz a -19"""

duet = Duet(pset_input)

print duet.perform_duet()