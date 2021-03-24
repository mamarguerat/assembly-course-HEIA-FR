; ****************************************************************************
; * @file   MP_3-0-ASM-f/MP_3-0_f.asm
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
message:    .cstring "Coucou"

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------

			mov.w	#message, r7			; adresse 1ere val.
			jmp		bcl1					; choix de l'algorithme
bcl1:
			mov.b	@r7+,r5					; déplacer la lettre à l'adresse r7, puis r7++
			cmp.b	#0,r5					; Comparer à la valeur 0 (caractère '\0' => fin de string)
        	jnz		p1						; 	si r5!=0 jump p1
        	jmp		fin						; 	sinon (r5=0) jump fin
p1:         mov.b	@r7, -1(r7)				; Copier la valeur à l'index i+1 dans l'index i (=> x[i] = x[i+1])
			jmp 	bcl1					; Recommencer la boucle
bcl2:
			mov.b	@r7+,r5					; déplacer la lettre à l'adresse r7, puis r7++
			cmp.b	#0,r5					; Comparer à la valeur 0 (caractère '\0' => fin de string)
        	jnz		p2						; 	si r5!=0 jump p2
        	jmp		fin						; 	sinon (r5=0) jump fin
p2:         inc.b	r5						; Ajouter 1 à la valeur de r5 (=> x[i] = x[i] + 1)
            mov.b	r5,-1(r7)				; Déplacer le nouveau caractère à l'adresse r7-1
			jmp 	bcl2					; Recommencer la boucle


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
            
