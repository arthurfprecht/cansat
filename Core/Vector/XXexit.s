/**************************************************
 *
 * __exit -- halts the system, non-debug version.
 *
 * Copyright 2006-2017 IAR Systems AB.
 *
 * $Revision: 112610 $
 *
 **************************************************/


        MODULE  ?__exit
        PUBLIC  __exit

        SECTION .text:CODE:NOROOT(2)

        THUMB
__exit
          NOCALL __exit

        MOVS    r1,#0x20
        LSLS    r1,r1,#12
        ADDS    r1,r1,#0x26    ; was: LDR  r1,=0x20026
        MOVS    r0,#0x18
        BKPT    0xAB
        B       __exit

        LTORG

        END
