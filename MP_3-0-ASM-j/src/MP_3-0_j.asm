; ****************************************************************************
; * @file   MP_3-0-ASM_sol1-j/MP_3-0_j.asm
; * @author	Martin MARGUERAT
; * @date	24-03-2021
; * @brief  Translate string to upper case
; ****************************************************************************/

            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.
message:    .cstring "~Bienvenue!"

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------

			mov.w #message, r7				; adresse 1ere val.

bcl:
			mov.b @r7+,r5					; déplacer la lettre à l'adresse r7, puis r7++
			cmp.b #0,r5						; Comparer à la valeur 0 (caractère '\0' => fin de string)
        	jnz p1							; 	si r5!=0 jump p1
        	jmp fin							; 	sinon (r5=0) jump fin
p1:         cmp.b #'a',r5					; Comparer à la valeur 0x61 (caractère 'a')
            jhs p2							; 	si r5>=0x61 jump p2
            jmp bcl							; 	sinon (r5<0x61) jump fin
p2:      	cmp.b #'z'+1,r5					; Comparer à la valeur 0x7B+1 (caractère 'z'+1)
            jlo p3							; 	si r5<0x7E jump p3
            jmp bcl							; 	sinon (r5>=0x7E) jump fin
p3:			sub.b #('a'-'A'),r5				; Soustraire la valeur 0x20 ('a'-'A' => 0x61-0x41 = 0x20)
			mov.b r5,-1(r7)					; Déplacer le nouveau caractère à l'adresse r7-1
			jmp bcl							; Recommencer la boucle


fin:		jmp		fin
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
            
