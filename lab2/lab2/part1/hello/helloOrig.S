@ hello.S: Hello world! in assembly
@
@ Authors: Matthew Tay mhtay@andrew.cmu.edu
@          Deeptaanshu Kumar deeptaan@andrew.cmu.edu
@          Kevin Brennan kbrennan@andrew.cmu.edu
@ Date:    11:53 10/10/2013
#include <bits/swi.h>

        .file   "hello.S"
        .text

        .global _start
_start:
        mov     r0, #1          @ int fd
        ldr     r1, =.hello     @ const void *buf
        mov     r2, #13         @ size_t count
        swi     WRITE_SWI	@ write to stdout
        mov     r0, #0
        swi     EXIT_SWI	@ exit with zero code
.hello:
        .ascii  "Hello World!\012\000"

