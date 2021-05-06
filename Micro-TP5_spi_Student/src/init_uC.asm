
; *****************************************************************************
; File:     inituC.asm
; Date:     20. April 2018
; Author    Buccella Pietro
;
; Routines to initialize the uController
; accessed out of C as standard extern Calls
;
; *****************************************************************************

      .cdecls C,NOLIST, "msp430.h"   ; Processor specific definitions


;============================================================================
; initChip_asm
;============================================================================
      .global initChip_asm            					; Declare symbol to be exported
      .sect ".text"                  					; Code is relocatable
initChip_asm:   .asmfunc


StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  			; Stop watchdog timer
			calla   #initGPIO_asm  						; Configure GPIO
			calla   #initCS_asm    						; Startup clock system with max DCO setting ~8MHz
			calla   #spi_init_asm  						; configure SPI
			reta

initGPIO_asm:

			bic.w   #LOCKLPM5,&PM5CTL0 					; Unlock I/O pins (IMPORTANT!!!!)
			clr.w   &PJOUT 								; Configure GPIO
			clr.w   &PBOUT 								; Configure GPIO
			clr.w   &PAOUT 								; Configure GPIO

			clr.w   &PBDIR 								; Port B (3 and 4) is switched to input direction
			clr.w   &PADIR 								; Port A (1 and 2) is switched to input direction

			bic.b   #BIT6,&PASEL1 						;  Configure P1.6 as input
;============================================================================
; #Partie 2: Configuration GPIO pour SPI du module eUSCI_B0
;============================================================================
			 						;  TBD Configure P1.7 as MISO
			 						;  TBD Configure P2.2 as UCB0CLK

			 						;  TBD Set P1.4/P1.3 as output manual CS (active low) for MCP23S08/TC77
									;  TBD Set P1.4/P1.3 initially inactive (high) manual CS (active low) for MCP23S08/TC77

			; LED handling
			 						;  TBD Set P1.0 as output
									;  TBD Set P4.6 as output
			reta

enableSPI_MOSI_asm:
			cmp.b   #1,R12 ; if (true == enable)
			jeq     CL1
			bic.b   #BIT6,&PASEL1 						;  Configure P1.6 as input
			jmp     CL2
CL1:		bis.b   #BIT6,&PASEL1 						;  Configure P1.6 as MOSI
CL2:  		reta

initCS_asm:

			mov.w   #0xa500,&FRCTL0						; Configure one FRAM waitstate as required by the device datasheet for MCLK operation beyond 8MHz _before_ configuring the clock system.
			mov.b   #CSKEY_H,&CSCTL0_H      			; Unlock CS registers
			mov.w   #DCOFSEL_6,&CSCTL1      			; Set DCO = 8MHz
			mov.w   #SELA__VLOCLK+SELS__DCOCLK+SELM__DCOCLK,&CSCTL2 ; Set ACLK=VLO SMCLK=MCLK=DCO
			clr.w   &CSCTL3 							;  Set all dividers to 1 (register cleared to 0...)
			clr.b   &CSCTL0_H               			; Lock CS registers
			reta

spi_init_asm:
;============================================================================
;#Partie 1: Configuration de l’eUSCI_B0
;============================================================================
			     										; TBD Put state machine in reset
			 											; TBD
														; TBD Data is captured on first UCLK edge Inactive clock polarity is low
														; TBD MSB is sent first;
														; TBD 3-pin SPI mode (manual STE control)
														; TBD SPI is configured as master
														; TBD eUSCI us used as SPI
														; TBD source clock is SMCLK
														; TBD fBitClock = fBRCLK/(UCBRx + 1) = 8MHz / 80
			 											; TBD Initialize USCI state machine
			reta


         	.endasmfunc

        	.end
