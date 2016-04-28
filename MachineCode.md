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
1101 MOV Rd = Op2

To do MOV r0, 0x3F200010 with immediate values:
Load f2 immediate + rot 12 right
Add 03 rot 4 right
add 10 no rot 

```
mov r0, 0x0f200000
add r0, 0x30000000
add r0, 0x00000010
        ----------
        0x3f200010
```

MOV r0, 0x0f200000
E = always
3 = 0011 = 00 + 1 (immedidate) + 1 (opcode high)

A = 1010 = 101 opcode lower + 0 no set cond
0 = 0000 = ignore first op reg

0 = 0000 = dst reg = r0
C = 1100 = rotate 12 right

F = 1111 = immediate value upper nibble
2 = 0010 = immediate value lower nibble

ADD r0, 0x30000000
E = always
2 = 0010 = 00 + 1 (imm) + 0 (opcode high)

8 = 1000 = 100 (opcode lower) + 0 (no cond)
0 = 0000 = adding value in r0

0 = 0000 = adding value to r0
4 = 0100 = rot right 4

0 = 0000 = imm value upper
3 = 0011 = imm value lower

ADD r0, 0x00000010
E = always
2 = 0010 00 + 1 + 0

8 = 1000 = 100 + 0
0 = 0000 = r0

0 = 0000 = r0
0 = 0000 = rot 0

1 = 0001 = imm upper
0 = 0000 = imm lower

mov r1, (r2 << 8)
e1 a0 12 02

mov r1, 0x8
e3 a0 10 08

### Subtraction
sub r0, 0x4 = e2 40 00 04
E = always
2 = 0010 = 00 + 1 (imm) + 0 (op high)

4 = 0100 = 010 (op low) + 0 (ignore cond)
0 = 0000 = src reg r0

0 = 0000 = dst reg r0
0 = 0000 = no rotate

0 = 0000 = imm upper
4 = 0100 = imm lower

### Bitwise
This is not used, should actually be AND NOT i.e. BIC
and r1, 0x00E00000 = E2 01 1C 0E
NB: 0x00E00000 = 0xE rot>> 12
E = always
2 = 0010 = 00 + 1 immediate + 0 upper opcode

0 = 0000 = 000 lower opcode + 0 ignore cond
1 = 0001 = op1 = r1

1 = 0001 = place into r1
C = 1100 = rot right 12

0 = 0000 = imm upper
E = 1110 = imm lower

bic r1, 0x00e00000 = e3 b1 1c 0e
E = always
3 = 0011 = 00 + 1 immediate + 1 upper opcode

b = 1100 = 110 lower opcode + 0 ignore cond
1 = 0001 = op1 = r1

1 = 0001 = place into r1
C = 1100 = rot right 12

0 = 0000 = imm upper
E = 1110 = imm lower

bic r1, 0x00038000 = mov r2, 0x7 then bic r1, (r2<<15)
NB: 38000 = 7 << 15
mov r2, 0x7
          = e1
1010 0000 = A0
0010 0000 = 20
0000 0111 = 07

bic r1, r2<<15 = e1 b1 17 82
e = always
1 = 0001 = 00 + 0 not imm + 1 upper

b = 1100 = 110 lower + 0 igore
1 = 0001 = op1 = r1
 
1 = 0001 = dst r1
7 = shift upper 4

8 = 1000 = shift lower 1 + 00 (logical left shift) + 0 (immediate shift)
2 = 0010 = shift by value in reg 2


or r1, 0x00200000 = E3 81 1C 02
NB: 0x00200000 = 0x1 rot>> 11 = 0x2 rot >> 12
E = always
3 = 0011 = 00 + 1 imm + 1 upper opcde

8 = 1000 = 100 lower op + 0 ignore cond
1 = 0001 = op1 = r1

1 = 0001 = dest = r1
C = 1100 = rot right 12

0 = 0000 = imm upper
2 = 0010 = imm lower

### Loads
ldr r1, [r0] = e5 90 10 00
E = always
5 = 0101 = 01 + 0 (immediate) + 1 (pre index)

9 = 1001 = up + word + no write back + load
0 = base reg r0 (address from r0)

1 = dst reg r1
0 = no offset

0 = no offset
0 = no offset

### Stores
1 nibble condition + 01 + bitflag (immediate?) + 4 bitflags + store/load bitflag + base reg + src/dest reg + offset

str r1, [r0] = e5 80 10 00
E = always
5 = 0101 = 01 + 0 (immediate) + 1 (pre index)

8 = 1000 = Up + word + no write back + store
0 = 0000 = base reg r0

1 = 0001 = src reg r1
0 = no offset

0 = no offset
0 = no offset

Alternate store machine code for clearing LED:
offset LED clear = 0x3F20002C

Assuming storing r1 to LED clear with:
r0 = 0
r2 = 00 02 C3 F2

```
  E cond always                }
+ 01 const         }           }
+ 1 shift mode     } 0111 = 7  } E7
+ 1 pre-indexed    }           }

+ 1 up             }           }
+ 0 word           } 1000 = 8  }
+ 0 no write-back  }           } 80
+ 0 store          }           }
+ regbase nibble (r0 = 0000)   }

+ reg src/dest nibble= r1 0001 }
+ shift # = 12 = C             } 1C

+ 0 shift # low bit   }        }
+ 11 type = rot right }  6     } 62
+ 0 fixed             }        }
+ offset reg = r2     } 2      }

```

## Resources
(Machine code lecture)[http://www.ee.ic.ac.uk/pcheung/teaching/ee2_computing/lecture_9.pdf]

