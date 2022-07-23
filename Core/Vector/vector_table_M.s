/**************************************************
 *
 * Part one of the system initialization code, contains low-level
 * initialization, plain thumb variant.
 *
 * Copyright 2008-2017 IAR Systems AB.
 *
 * $Revision: 160895 $
 *
 **************************************************/

;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol __iar_program_start or
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

        MODULE  ?vector_table

        AAPCS INTERWORK, VFP_COMPATIBLE, RWPI_COMPATIBLE
        PRESERVE8


          CFI Names cfiNames0
          CFI StackFrame CFA R13 DATA
          CFI Resource R0:32, R1:32, R2:32, R3:32, R4:32, R5:32, R6:32, R7:32
          CFI Resource R8:32, R9:32, R10:32, R11:32, R12:32, R13:32, R14:32
          CFI EndNames cfiNames0
        
          CFI Common cfiCommon0 Using cfiNames0
          CFI CodeAlign 2
          CFI DataAlign 4
          CFI ReturnAddress R14 CODE
          CFI CFA R13+0
          CFI R0 Undefined
          CFI R1 Undefined
          CFI R2 Undefined
          CFI R3 Undefined
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R6 SameValue
          CFI R7 SameValue
          CFI R8 SameValue
          CFI R9 SameValue
          CFI R10 SameValue
          CFI R11 SameValue
          CFI R12 Undefined
          CFI R14 SameValue
          CFI EndCommon cfiCommon0
        
        
        
        
        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
		EXTERN  systemInit        
        PUBLIC  __vector_table

        EXTERN NMI_Handler
        EXTERN HardFault_Handler
        EXTERN MemManage_Handler
        EXTERN BusFault_Handler
        EXTERN UsageFault_Handler
        EXTERN SVC_Handler
        EXTERN DebugMon_Handler
        EXTERN PendSV_Handler
        EXTERN SysTick_Handler

        DATA

__iar_init$$done:               ; The vector table is not needed
                                ; until after copy initialization is done

__vector_table
        DCD     sfe(CSTACK)
        DCD     __iar_program_start

        DCD     NMI_Handler
        DCD     HardFault_Handler
        DCD     MemManage_Handler
        DCD     BusFault_Handler
        DCD     UsageFault_Handler
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     SVC_Handler
        DCD     DebugMon_Handler
        DCD     0
        DCD     PendSV_Handler
        DCD     SysTick_Handler
    ; External Interrupts
        DCD     __irq_wwdg           ; Window Watchdog
        DCD     __irq_pvd            ; PVD through EXTI Line detect
        DCD     __irq_tamper         ; Tamper
        DCD     __irq_rtc            ; RTC
        DCD     __irq_flash          ; Flash
        DCD     __irq_rcc            ; RCC
        DCD     __irq_exti0          ; EXTI Line 0
        DCD     __irq_exti1          ; EXTI Line 1
        DCD     __irq_exti2          ; EXTI Line 2
        DCD     __irq_exti3          ; EXTI Line 3
        DCD     __irq_exti4          ; EXTI Line 4
        DCD     __irq_dma1_channel1  ; DMA1 Channel 1
        DCD     __irq_dma1_channel2  ; DMA1 Channel 2
        DCD     __irq_dma1_channel3  ; DMA1 Channel 3
        DCD     __irq_dma1_channel4  ; DMA1 Channel 4
        DCD     __irq_dma1_channel5  ; DMA1 Channel 5
        DCD     __irq_dma1_channel6  ; DMA1 Channel 6
        DCD     __irq_dma1_channel7  ; DMA1 Channel 7
        DCD     __irq_adc         ; ADC1 & ADC2
        DCD     __irq_usb_hp_can_tx  ; USB High Priority or CAN1 TX
        DCD     __irq_usb_lp_can_rx0 ; USB Low  Priority or CAN1 RX0
        DCD     __irq_can_rx1       ; CAN1 RX1
        DCD     __irq_can_sce       ; CAN1 SCE
        DCD     __irq_exti9_5        ; EXTI Line 9..5
        DCD     __irq_tim1_brk       ; TIM1 Break
        DCD     __irq_tim1_up        ; TIM1 Update
        DCD     __irq_tim1_trg_com   ; TIM1 Trigger and Commutation
        DCD     __irq_tim1_cc        ; TIM1 Capture Compare
        DCD     __irq_tim2           ; TIM2
        DCD     __irq_tim3           ; TIM3
        DCD     __irq_tim4           ; TIM4
        DCD     __irq_i2c1_ev        ; I2C1 Event
        DCD     __irq_i2c1_er        ; I2C1 Error
        DCD     __irq_i2c2_ev        ; I2C2 Event
        DCD     __irq_i2c2_er        ; I2C2 Error
        DCD     __irq_spi1           ; SPI1
        DCD     __irq_spi2           ; SPI2
        DCD     __irq_usart1         ; USART1
        DCD     __irq_usart2         ; USART2
        DCD     __irq_usart3         ; USART3
        DCD     __irq_exti15_10      ; EXTI Line 15..10
        DCD     __irq_rtcalarm       ; RTC Alarm through EXTI Line
        DCD     __irq_usbwakeup      ; USB Wakeup from suspend

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        LDR     R0, =systemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0
        
        PUBWEAK __irq_wwdg
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_wwdg
        B __irq_wwdg

        PUBWEAK __irq_pvd
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_pvd
        B __irq_pvd

        PUBWEAK __irq_tamper
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_tamper
        B __irq_tamper

        PUBWEAK __irq_rtc
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_rtc
        B __irq_rtc

        PUBWEAK __irq_flash
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_flash
        B __irq_flash

        PUBWEAK __irq_rcc
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_rcc
        B __irq_rcc

        PUBWEAK __irq_exti0
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_exti0
        B __irq_exti0

        PUBWEAK __irq_exti1
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_exti1
        B __irq_exti1

        PUBWEAK __irq_exti2
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_exti2
        B __irq_exti2

        PUBWEAK __irq_exti3
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_exti3
        B __irq_exti3

        PUBWEAK __irq_exti4
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_exti4
        B __irq_exti4

        PUBWEAK __irq_dma1_channel1
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_dma1_channel1
        B __irq_dma1_channel1

        PUBWEAK __irq_dma1_channel2
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_dma1_channel2
        B __irq_dma1_channel2

        PUBWEAK __irq_dma1_channel3
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_dma1_channel3
        B __irq_dma1_channel3

        PUBWEAK __irq_dma1_channel4
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_dma1_channel4
        B __irq_dma1_channel4

        PUBWEAK __irq_dma1_channel5
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_dma1_channel5
        B __irq_dma1_channel5

        PUBWEAK __irq_dma1_channel6
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_dma1_channel6
        B __irq_dma1_channel6

        PUBWEAK __irq_dma1_channel7
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_dma1_channel7
        B __irq_dma1_channel7

        PUBWEAK __irq_adc
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_adc
        B __irq_adc

        PUBWEAK __irq_usb_hp_can_tx
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_usb_hp_can_tx
        B __irq_usb_hp_can_tx

        PUBWEAK __irq_usb_lp_can_rx0
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_usb_lp_can_rx0
        B __irq_usb_lp_can_rx0

        PUBWEAK __irq_can_rx1
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_can_rx1
        B __irq_can_rx1

        PUBWEAK __irq_can_sce
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_can_sce
        B __irq_can_sce

        PUBWEAK __irq_exti9_5
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_exti9_5
        B __irq_exti9_5

        PUBWEAK __irq_tim1_brk
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_tim1_brk
        B __irq_tim1_brk

        PUBWEAK __irq_tim1_up
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_tim1_up
        B __irq_tim1_up

        PUBWEAK __irq_tim1_trg_com
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_tim1_trg_com
        B __irq_tim1_trg_com

        PUBWEAK __irq_tim1_cc
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_tim1_cc
        B __irq_tim1_cc

        PUBWEAK __irq_tim2
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_tim2
        B __irq_tim2

        PUBWEAK __irq_tim3
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_tim3
        B __irq_tim3

        PUBWEAK __irq_tim4
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_tim4
        B __irq_tim4

        PUBWEAK __irq_i2c1_ev
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_i2c1_ev
        B __irq_i2c1_ev

        PUBWEAK __irq_i2c1_er
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_i2c1_er
        B __irq_i2c1_er

        PUBWEAK __irq_i2c2_ev
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_i2c2_ev
        B __irq_i2c2_ev

        PUBWEAK __irq_i2c2_er
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_i2c2_er
        B __irq_i2c2_er

        PUBWEAK __irq_spi1
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_spi1
        B __irq_spi1

        PUBWEAK __irq_spi2
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_spi2
        B __irq_spi2

        PUBWEAK __irq_usart1
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_usart1
        B __irq_usart1

        PUBWEAK __irq_usart2
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_usart2
        B __irq_usart2

        PUBWEAK __irq_usart3
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_usart3
        B __irq_usart3

        PUBWEAK __irq_exti15_10
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_exti15_10
        B __irq_exti15_10

        PUBWEAK __irq_rtcalarm
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_rtcalarm
        B __irq_rtcalarm

        PUBWEAK __irq_usbwakeup
        SECTION .text:CODE:REORDER:NOROOT(1)
__irq_usbwakeup
        B __irq_usbwakeup

        END
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/