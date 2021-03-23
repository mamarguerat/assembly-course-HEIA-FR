; ****************************************************************************
; * @file    Micro-TP2_template.asm
; * @author  MARGUERAT - SCHUPBACH
; * @date    18.03.2021
; * @brief   TP2 - Arithm�tique et assembleur
; *
; ****************************************************************************/
;-------------------------------------------------------------------------------

        .cdecls C,LIST,"msp430.h"       ; Include device header file
        .global RESET

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

; d�but du programme pour le TP2
loop:
; - TP2: 4.2 Move
        mov.b  #55h, R5                  ; Move byte
        mov.w  #0xAA55, R10              ; Move word

; - TP2: 4.3 Addition
        mov.b  #11h, R6                  ; byte sans d�passement
        add.b  R5, R6                    ;
        addc.b R6, R6                    ;
        addc.b R6, R6                    ; d�passement...
        addc.b R6, R6                    ; byte avec d�passement
        bic.w	#0xFFFF, SR				; Clear all SR bits
        nop                             ; to show result before next instr.

        ; a) word sans d�passement
        mov.w	#0245h, R6				; word sans d�passement
        add.w	R6, R6					;
        addc.w	#828Ch, R6				;

        ; b) word avec d�passement
        addc.w	R6, R6					; d�passement...
        addc.w	R6, R6					; word avec d�passement
        bic.w	#0xFFFF, SR				; Clear all SR bits
        nop                             ; to show result before next instr.

; - TP2: 4.4 Soustraction
        ; a) byte sans d�passement et sans r�port
        mov.b	#7Ch, R6				; byte sans d�passement
        sub.b	#70h, R6				;

        ; b) byte sans d�passement, avec r�port
        subc.b	#0Ah, R6				; r�port

        ; c) word sans d�passement  et sans r�port
        sub.w	#5000h, R6				;

        ; d) word sans d�passement, avec r�port
        subc.w	#0A00h, R6				; r�port
        bic.w	#0xFFFF, SR				; Clear all SR bits
        nop                             ; to show result before next instr.

; - TP2: 4.5 Comparaison
		mov.w	#0x7851, R6				; positive word
		cmp.w	#0x7900, R6				; x>R6
		cmp.w	#0x7851, R6				; x=R6
		cmp.w	#0x112A, R6				; X<R6
        bic.w	#0xFFFF, SR				; Clear all SR bits
        nop                             ; to show result before next instr.

; - TP2: 4.6 Test
		mov.w	#0x1111, R6				; positive word
		tst.w	R6						; R6>0
		mov.w	#0x0, R6				; O
		tst.w	R6						; R6=0
		mov.w	#0xA000, R6				; negative word
		tst.w	R6						; R6<0
        bic.w	#0xFFFF, SR				; Clear all SR bits
        nop                             ; to show result before next instr.

; - TP2: 4.7 Instr. Logiques
		mov.b	#0x42, R6				; positive byte
		and.b	#0xF0, R6				; bit3..0 -> 0 => R6=0x40
		mov.b	#0x42, R6				; positive byte
		bic.b	#0x0F, R6				; bit3..0 clear => R6=0x40
		mov.w	#0x5555, R6				; positive word
		xor.w	#0xFFFF, R6				; bit15..0 invert => R6=0xAAAA
		bis.w	#0xF000, R6				; bit15..12 set => R6=0xFAAA
		mov.w	#0xFFEE, R6				; negative word
		bit.w	#0x8000, R6				; bit15 test
        bic.w	#0xFFFF, SR				; Clear all SR bits
        nop                             ; to show result before next instr.


; - TP2: 4.8 Additions/Soustractions sign� 16bits
        ; a) Addition
		mov.w	#0x7FFF, R6				; positive word
		add.w	#0x0001, R6				; Addition avec d�passement sans limitation: R6=0x8000
		; V=1, N=1
        bic.w	#0xFFFF, SR				; Clear all SR bits
        nop                             ; to show result before next instr.

        ; b) Soustraction
		mov.w	#0x8000, R6				; negative word
		sub.w	#0x0001, R6				; Soustration avec d�passement sans limitation: R6=0x7FFF
		; V=1, N=0
        bic.w	#0xFFFF, SR				; Clear all SR bits
        nop                             ; to show result before next instr.

        ; c) Limitation de l'addition au valeur max
		mov.w	#0x7FFF, R6				; positive word
		add.w	#0x0001, R6				; Addition avec d�passement sans limitation
		jl		limitAdd				; S'il y a d�passement, limiter l'addition
		jn		limitAdd				; Si le nombre est n�gatif, limiter l'addition
finishAdd:
        bic.w	#0xFFFF, SR				; Clear all SR bits
        nop                             ; to show result before next instr.

        ; d) Limitation la soustraction au min value
		mov.w	#0x8000, R6				; negative word
		sub.w	#0x0001, R6				; Soustration avec d�passement sans limitation
		jge		finishSub				; S'il y a pas de d�passement, sauter � la fin de la soustraction
		jn		finishSub				; Si le nombre est n�gatif, sauter � la fin de la soustraction
		mov.w	#0x8000, R6				; Limitation de la soustraction
finishSub:
        bic.w	#0xFFFF, SR				; Clear all SR bits
        nop                             ; to show result before next instr.
        ; e) Op�rations sur 32bits



; fin du programme pour le TP2

        jmp loop                        ; jump to current location '$'
                                        ; (endless loop)
limitAdd:
		mov.w	#0x7FFF, R6				; Limitation de l'addition
		jmp		finishAdd				; sauter � la fin de l'addition
        nop                             ; necessary for MSP430FR5969





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
