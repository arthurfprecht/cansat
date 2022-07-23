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


        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .text:CODE:NOROOT(2)

        EXTERN  __cmain
        PUBLIC  __iar_rwpi_program_start

        THUMB
__iar_rwpi_program_start:

;;;
;;; SP and SB must be set up properly before this point
;;;
        BL      __cmain

        END
