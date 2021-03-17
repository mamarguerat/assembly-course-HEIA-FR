; ****************************************************************************
; * @file   MP_3-3-ASM/MP_3-3.asm
; * @author	Martin MARGUERAT
; * @date	16-03-2021
; * @brief  Calculate data sum and means in assembler
; ****************************************************************************/

;-------------------------------------------------------------------------------

        .cdecls C,LIST,"msp430.h"       ; Include device header file
        .global RESET
;-------------------------------------------------------------------------------
NB_VAL:	.equ	16						; number of Values constant
;-------------------------------------------------------------------------------
		.sect	.data					; section of initialized data

Values:	.byte	122,23,31,27,54,-2,-17	; table of 8-bit signed numbers
		.byte	-5,112,13,56,65,21,87
		.byte	-32,-54

;-------------------------------------------------------------------------------
		.sect	.bss					; section of uninitialized data
		.align 2						; specify a word alignement

Sum:	.space	2 						; reserve 1 word for variable Sum
Mean:	.space	1						; reserve 1 byte for variable Mean

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

main:	nop
; a) calculte the sum of each values, result in Sum
		mov.w	#0,r5					; initialize index with 0
		mov.w	#0,r6					; initialize sum with 0
		mov.w	#Values,r7				; load 1st Values address in R7
bcl:	mov.b	@r7,r8					; move value (address R7) in R8 and increment R7
		inc.w	r7						; increment value address
		bit.b	#10000000b,r8			; check for a negative sign
		jz		doSum					; Jump to doSum if it's a positive number
		bis.w	#1111111100000000b,r8	; convert 8bit negative to 16bit negative
doSum:	add.w	r8,r6					; add r8 to sum
		inc.w	r5						; increment index
		cmp.w	#NB_VAL,r5				; compare index and NB_VAL
		jn		bcl						; jump to bcl if index is less than NB_VAL
		mov.w	r6,Sum					; move sum in memory

; b) calculate the Mean = Sum / NB_VAL
		clrc							; clear carry bit
		rrc.w	r6						; divide sum by 2 (total: /2)
		rra.w	r6						; divide sum by 2 (total: /4)
		rra.w	r6						; divide sum by 2 (total: /8)
		rra.w	r6						; divide sum by 2 (total: /16)
		mov.w	r6,Mean					; move mean in memory

fin:	jmp		fin
		nop
        
;-------------------------------------------------------------------------------
;           Stack Pointer definition
;-------------------------------------------------------------------------------
        .global __STACK_END
        .sect 	.stack

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
        .sect   ".reset"                ; MSP430 RESET Vector
        .short  RESET
        .end
