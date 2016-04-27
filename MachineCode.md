# Writing in ARM7 Machine Code
## Instructions
### Branches
1 nibble condition + 101 + (1 if set r14 for return) + 3 byte signed offset

Unconditional jumps:
EA = branch unconditionally
EB = branch unconditionally, set r14 for return address

### Arithmetic
1 nibble condition + 00 + bit flag (shift) + opcode + bit flag (set cond) + reg 1 + reg dest + operand 2

Opcodes:
0000 AND
0100 ADD

## Resources
(Machine code lecture)[http://www.ee.ic.ac.uk/pcheung/teaching/ee2_computing/lecture_9.pdf]

