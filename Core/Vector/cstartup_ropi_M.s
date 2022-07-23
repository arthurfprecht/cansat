/**************************************************
 *
 * Part one of the system initialization code, contains low-level
 * initialization, plain thumb variant.
 *
 * Copyright 2008-2017 IAR Systems AB.
 *
 * $Revision: 112610 $
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

        ;; Forward declaration of section.
        SECTION CSTACK:DATA:NOROOT(3)

        PUBLIC  __iar_ropi_program_start

        EXTERN  __cmain
        EXTWEAK __iar_init_core
        EXTWEAK __iar_init_vfp

        SECTION .text:CODE:REORDER(2)
        THUMB
__iar_ropi_program_start:
        LDR     R3, =sfe(CSTACK)
        LSRS    R3,R3,#3
        LSLS    R3,R3,#3
        MOV     SP, R3

          FUNCALL __iar_ropi_program_start, __iar_init_core
        BL      __iar_init_core
          FUNCALL __iar_ropi_program_start, __iar_init_vfp
        BL      __iar_init_vfp

          FUNCALL __iar_ropi_program_start, __cmain
        BL      __cmain

        END
