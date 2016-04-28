# baremetalpi2
Bare metal programming on the Raspberry Pi 2, from the ground up!

# Resources
- [Machine code hand-assembly lecture](http://www.ee.ic.ac.uk/pcheung/teaching/ee2_computing/lecture_9.pdf)
- [In-depth machine code](http://bear.ces.cwru.edu/eecs_382/ARM7-TDMI-manual-pt2.pdf)
- [dwelch's incredible bare metal programming guides](https://github.com/dwelch67/raspberrypi)
- [cherryhex, C++ converter for ASCII HEX > raw binary files](https://github.com/SgtCoDFish/cherryhex)

# Existing Code
## ledclear
Language: ARM7 hex machine code, hand assembled

Turns off the LEDs on the pi and then does nothing. Good introduction to bare metal programming in raw machine code. Uses [cherryhex](https://github.com/SgtCoDFish/cherryhex) to convert .hex file to a binary file.
