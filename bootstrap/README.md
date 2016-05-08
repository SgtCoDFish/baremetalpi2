# bootstrap
Bootstrapping a compiler from the ground up.

# Stages
## cherryhex
[cherryhex](https://github.com/SgtCoDFish/cherryhex) is the program used to convert hex files (+ comments) into binary files which can be run as kernel images on the Pi. It's the only software step of the process which isn't run on the Pi at first, so the first step of bootstrapping a compiler is to recreate cherryhex (albeit a slightly simplified version) in ARMv7 machine code.

