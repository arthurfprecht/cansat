/**************************************************
 *
 * Part one of the system initialization code, contains low-level
 * initialization, plain thumb variant.
 *
 * Copyright 2008-2017 IAR Systems AB.
 *
 * $Revision: 163563 $
 *
 **************************************************/

;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY, which
; is where to find the SP start value.
; If vector table is not located at address 0, the user has to initialize the  NVIC vector
; table register (VTOR) before using interrupts.
;
; Cortex-M version
;

        MODULE  ?cstartup


        PUBLIC  __iar_program_start
        EXTERN  __cmain
        EXTERN  __vector_table
        EXTWEAK __iar_init_core
        EXTWEAK __iar_init_vfp

        SECTION .text:CODE:REORDER(2)
        THUMB
__iar_program_start:
#if __ARM_ISA_THUMB >= 2
        mov32   r0, #0xfef5eda5
#else
#ifndef __NO_LITERAL_POOL__
        ldr     r0, =0xfef5eda5
#else
        movs    r0,     #0xfe
        lsls    r0, r0, #8
        adds    r0, r0, #0xf5
        lsls    r0, r0, #8
        adds    r0, r0, #0xed
        lsls    r0, r0, #8
        adds    r0, r0, #0xa5
#endif
#endif
        mov     r1, r0
        push    {r0, r1}
        mov     r0, sp
        msr     PSP, r0

        FUNCALL __iar_program_start, __iar_init_core
        BL      __iar_init_core
        FUNCALL __iar_program_start, __iar_init_vfp
        BL      __iar_init_vfp

        FUNCALL __iar_program_start, __cmain
        BL      __cmain

        REQUIRE __vector_table


        END
