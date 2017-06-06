# ARM7 Machine Code
Manual instruction assembly is a bit strange to see at first. This document certainly doesn't help; it's more of a notepad used while a few instructions were hand-assembled.

Look at this [machine code hand-assembly lecture](http://www.ee.ic.ac.uk/pcheung/teaching/ee2_computing/lecture_9.pdf) for a more readable example.


### ledclear.hex
1 nibble condition + 00 + bit flag (immediate?) + opcode + bit flag (set cond?) + reg 1 + reg dest + operand 2

Initial load of LED address into memory:
```
mov r0,     0x0f200000
add r0, r0, 0x30000000
add r0, r0, 0x00000010
            ----------
            0x3f200010
```

mov r0, 0x0f200000
```
E = always
3 = 0011 = 00 + 1 (immedidate) + 1 (opcode high)

A = 1010 = 101 opcode lower + 0 no set cond
0 = 0000 = ignore first op reg

0 = 0000 = dst reg = r0
6 = 0110 = rotate 6*2=12 right

F = 1111 = immediate value upper nibble
2 = 0010 = immediate value lower nibble
```

add r0, r0, 0x30000000
```
E = always
2 = 0010 = 00 + 1 (imm) + 0 (opcode high)

8 = 1000 = 100 (opcode lower) + 0 (no cond)
0 = 0000 = adding value in r0

0 = 0000 = adding value to r0
2 = 0010 = rot right 2 * 2 = 4

0 = 0000 = imm value upper
3 = 0011 = imm value lower
```

add r0, r0, 0x00000010
```
E = always
2 = 0010 = 00 + 1 + 0

8 = 1000 = 100 + 0
0 = 0000 = r0

0 = 0000 = r0
0 = 0000 = rot 0

1 = 0001 = imm upper
0 = 0000 = imm lower
```

mov r1, lsl r2, 0x8
`e1 a0 14 02`

mov r2, 0x7
`e3 a0 20 07`

### Subtraction
sub r0, r0, 0x4 = e2 40 00 04
```
E = always
2 = 0010 = 00 + 1 (imm) + 0 (op high)

4 = 0100 = 010 (op low) + 0 (ignore cond)
0 = 0000 = src reg r0

0 = 0000 = dst reg r0
0 = 0000 = no rotate

0 = 0000 = imm upper
4 = 0100 = imm lower
```

### Bitwise
bic r1, 0x00e00000 = e3 c1 16 0e
```
E = always
3 = 0011 = 00 + 1 immediate + 1 upper opcode

c = 1101 = 110 lower opcode + 1 cond
1 = 0001 = op1 = r1

1 = 0001 = place into r1
6 = 0110 = rot right 6 * 2 = 12

0 = 0000 = imm upper
E = 1110 = imm lower
```

bic r1, 0x00038000 = mov r2, 0x7 then bic r1, lsl r2, 15

orr r1, 0x00200000 = E3 81 16 02
```
E = always
3 = 0011 = 00 + 1 imm + 1 upper opcde

8 = 1000 = 100 lower op + 0 ignore cond
1 = 0001 = op1 = r1

1 = 0001 = dest = r1
6 = 0110 = rot right 6 * 2 = 12

0 = 0000 = imm upper
2 = 0010 = imm lower
```

### Loads
ldr r1, [r0] = e5 90 10 00
```
E = always
5 = 0101 = 01 + 0 (immediate) + 1 (pre index)

9 = 1001 = up + word + no write back + load
0 = base reg r0 (address from r0)

1 = dst reg r1
0 = no offset

0 = no offset
0 = no offset
```

### Stores
1 nibble condition + 01 + bitflag (immediate?) + 4 bitflags + store/load bitflag + base reg + src/dest reg + offset

str r1, [r0] = e5 80 10 00
```
E = always
5 = 0101 = 01 + 0 (immediate) + 1 (pre index)

8 = 1000 = Up + word + no write back + store
0 = 0000 = base reg r0

1 = 0001 = src reg r1
0 = no offset

0 = no offset
0 = no offset
```
