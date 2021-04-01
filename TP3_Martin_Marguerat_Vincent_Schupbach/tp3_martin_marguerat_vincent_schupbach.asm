; ****************************************************************************
; * @file    TP3_Martin_Marguerat_Vincent_Schupbach/tp3_martin_marguerat_vincent_schupbach.asm
; * @author  MARGUERAT - SCHUPBACH
; * @date    01.04.2021
; * @brief   TP4 - Stack et fonctions
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

;----- d�but du programme pour le TP2 ---------

; D�claration des tableaux
arr1:	.int	1, 2, 3, 4, 1, 2, 3, 4	; le premier tableau
arr2:	.int	1, 1, 3, 1, 1, 1, 5, 4	; le deuxi�me tableau

; D�claration des variables utilis�es
			mov		#0, r4				; i = 0
			mov		#0, r5				; produitScalaire = 0
			mov		#arr1, r6			; adresse de la premi�re valeur du premier tableau
			mov		#arr2, r7			; adresse de la premi�re valeur du deuxi�me tableau

; Boucle principale
Loop:		cmp		#7, r4				; comparer la valeur 8 � i
			jnc		endoffile			; si 7 < i (i > 7), fin du programme
			push.b	@r6+				; mise de la valeur arr1[i] sur la pile
			push.b	@r7+				; mise de la valeur arr2[i] sur la pile
			call	#m8ax8b				; appel de la fonction m8ax8b
			pop		r9					; r�cup�rer le deuxi�me nombre (inutile)
			pop		r8					; r�cup�rer le r�sultat de l'op�ration
			add		r8, r5				; produitScalaire = produitScalaire + r�sultat
			inc		r4					; i = i + 1
			jmp		Loop				; recommencer la boucle

; Fonction m8ax8b
; 	Cette fonction r�cup�re 2 valeurs enti�res positives a et b et retourne
;	la multiplication de ces deux nombres
;		INPUT:	SP+4 -> nombre 1 (entier non-sign� 8bits)
;				SP+2 -> nombre 2 (entier non-sign� 8bits)
;		OUTPUT:	SP+4 <- produit des nombres (entier non-sign� 16bits)
;				SP+2 <- nombre 2 (entier non-sign� 8bits)
m8ax8b:		mov		4(r1), r8			; r�cup�rer la valeur 1 (a)
			mov		2(r1), r9			; r�cuperer la valeur 2 (b)
			mov		#0, r10				; reste = 0
boucle:		clrc						; clear carry (C=0)
			rrc		r9					; d�calage de b vers la droite (b = b/2)
			jz		sortie				; si b = 0, executer les op�rations de sortie de la fonction
			jnc		saut				; saut s'il n'y a pas de retenue � ajouter
			add		r8, r10				; ajout de la valeur de retenue
saut:		rla		r10					; d�calage de a vers la droite (a = a*2)
			jmp		boucle				; recommencer la boucle
sortie:		add		r10, r8				; ajout de la retenue au total
			mov		r8, 4(r1)			; mise du r�sultat dans la pile
			ret							; fin de la fonction

;----- fin du programme pour le TP2 -----------

endoffile:	jmp endoffile               ; jump to current location '$'
            nop                         ; (endless loop)





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
