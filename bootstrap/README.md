# bootstrap
Bootstrapping a compiler from the ground up. We write a program in machine code which "compiles" itself over UART on a Raspberry Pi 2.

This process is based loosely on the process described by [Edmund Grimley Evans](https://web.archive.org/web/20160604035203/http://homepage.ntlworld.com/edmund.grimley-evans/bcompiler.html).

There are several differences compared with his process however:

- ARM machine code rather than x86
- UART for IO rather than file-based system calls
- "Bare metal" without an OS or executable file formats, compared to Linux + ELF
- Targeting a different goal

# Stages
## Stage 1 - cherryhex
✅

[cherryhex](https://github.com/SgtCoDFish/cherryhex) is a program used to convert .hex files (+ comments) into binary files which can be run as kernel images on the Pi. This is the only software step of the process which isn't under our control, so the first step of bootstrapping a compiler for an architecture is to recreate a program like cherryhex in machine code (albeit a slightly simplified version).

The stage1 directory contains two .hex programs which recreate cherryhex. The first, `cherryhex.hex` does just that, but has the issue that it cannot handle uppercase ASCII hex characters, and will produce an incorrect program in-memory if uppercase is used at any point. `cherryhex2.hex` fixes this.

Both versions are capable of "reading" themselves and recreating themselves in memory. This means that if a more advanced language is to be parsed than the raw hex we're currently using, we can use these programs to do it.

## Stage 2 - cherryasm
Writing in machine code is not a long term goal because it takes a long time and can be extremely hard to modify after writing thanks to the relative addresses you're forced to use. The next stage is to write a basic assembler, to make life easier for ourselves.

The main advantage of having an assembler is the relaxation of requirements on code positioning; we can support instruction labels and then jump to or from the labels, leaving the offsets to be calculated when the program is assembled. We'll ignore anything else an assembler might do because "cherryasm" must at first be written in machine code so simplicity is key.

We'll use a simple syntax:

```
#     - rest of line is a comment (until \n)
Hex   - as normal with machine code
xAB:  - Define a label at this point where AB is a 2-digit hexadecimal   
        number which makes up the name of the label.
jxAB  - Jump to a previously defined label
```

To keep cherryasm simple, all labels must be defined before they are referenced in the file. We limit labels to 2 symbols to make them easier to parse. It also forgoes many types of error checking, again for simplicity.
