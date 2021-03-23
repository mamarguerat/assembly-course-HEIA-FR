; ****************************************************************************
; * @file   MP_3-0-i/main.asm
; * @author	Martin MARGUERAT
; * @date	23-03-2021
; * @brief  Calculate a quotient and a modulus value of 2 words
; ****************************************************************************/

        .cdecls C,LIST,"msp430.h"       ; Include device header file
        .global RESET
;-------------------------------------------------------------------------------
		.sect	.data					; section of initialized data

;-------------------------------------------------------------------------------
		.sect	.bss					; section of uninitialized data
		.align 2						; specify a word alignement

Quotient:	.space	2					; reserve 1 word for variable Quotient
Modulus:	.space	2					; reserve 1 word for variable Reste

;-------------------------------------------------------------------------------
        .text                           ; Assemble into program memory
        .retain                         ; Override ELF conditional linking
                                        ; and retain current section
        .retainrefs                     ; Additionally retain any sections
                                        ; that have references to current
                                        ; section
;-------------------------------------------------------------------------------

RESET:   mov.w   #__STACK_END,SP        ; Initialize stackpointer
StopWDT: mov.w   #WDTPW|WDTHOLD,&WDTCTL ; Stop watchdog timer

;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
main:	nop
            mov.w	#0x15AF, r6				; X value
            mov.w	#0x4FFF, r7				; Y value
            mov.w	#0x0, r8                ; index
divide:		cmp.w	r6, r7
			jl		end						; jump to end if y < x
			sub.w	r6, r7					; y = y-x
			inc.w	r8						; i++
			jmp		divide
end:		mov.w	r8, Quotient			; i = quotient	Y/X
			mov.w	r7, Modulus				; y = modulus	Y%X
loop:		jmp		loop					; garde-fou
			nop

;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            
