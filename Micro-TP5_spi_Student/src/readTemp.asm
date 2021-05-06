
; *****************************************************************************
; File:     Port1.asm
; Date:     20. April 2018
; Author    Buccella Pietro
;
; Routines to read the temperature from TC77 sensor
; accessed out of C as standard extern Calls
;
; *****************************************************************************

      .cdecls C,NOLIST, "msp430.h"   ; Processor specific definitions


;============================================================================
; initChip_asm
;============================================================================
      .global tc77_readTemp_asm           	 	; Declare symbol to be exported
      .sect ".text"                  		 	; Code is relocatable
tc77_readTemp_asm:   .asmfunc
			calla   #setTC77_CS_asm			 	;
			calla   #spi_read16bitData_asm	 	;
			calla   #resetTC77_CS_asm			;
			bit.w   #4,R12						;
			jne     CL1Temp						;
			mov.w   #0x7fff,R12					;
			jmp     CL2Temp						;
CL1Temp:	rram.w  #3,R12						;
CL2Temp: 	reta								;

setTC77_CS_asm:
			bic.b   #BIT3,&P1OUT				;
			reta								;

spi_read16bitData_asm:
			clr.w   &UCB0TXBUF					;
CL5Data:	bit.w   #BIT0,&UCB0IFG				;
			jeq     CL5Data						;

			bic.w   #BIT0+BIT1,&UCB0IFG         ;
			mov.w   &UCB0RXBUF,R12				;
			mov.b   R12,R12						;
			swpb    R12							;
			clr.w   &UCB0TXBUF					;
CL6Data:	bit.w   #BIT0,&UCB0IFG				;
			jeq     CL6Data						;
			bic.w	#BIT0+BIT1,&UCB0IFG			;
			bis.w   &UCB0RXBUF,R12				;
			reta								;

resetTC77_CS_asm:
			bis.b   #BIT3,&P1OUT				;
			reta								;

         	.endasmfunc

        	.end
