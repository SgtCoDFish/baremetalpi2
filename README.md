# baremetalpi2
Bare metal programming on the Raspberry Pi, from the ground up!

This repository is intended to be a semi-educational logbook of my experiments and fun with bare metal programming on the Raspberry Pi.

Ideally readers should be able to follow a path to compiling a non-trivial program entirely on bare metal, or just browse and play with smaller bare-metal examples.

This repository is heavily inspired by [dwelch's incredible bare metal programming guides](https://github.com/dwelch67/raspberrypi).

# Sub-Projects
## ledclear
Board: Raspberry Pi 2
Language: ARM7 Little-endian Hex machine code "compiled" with [cherryhex](https://github.com/SgtCoDFish/cherryhex)

Turns off the LEDs on the pi2 and then does nothing. Good introduction to bare metal programming in raw machine code. 

# Resources
- [Machine code hand-assembly lecture](http://www.ee.ic.ac.uk/pcheung/teaching/ee2_computing/lecture_9.pdf)
- [In-depth machine code](http://bear.ces.cwru.edu/eecs_382/ARM7-TDMI-manual-pt2.pdf)
- [dwelch's bare metal programming guides](https://github.com/dwelch67/raspberrypi)
- [cherryhex, C++ converter for ASCII HEX > raw binary files](https://github.com/SgtCoDFish/cherryhex)
