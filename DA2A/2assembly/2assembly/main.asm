;
; 2assembly.asm
; Author : hierholz
;


.org 0

.include <m328pdef.inc> ;defining chip 
	
	SBI DDRB, 5			;turns off LED D1
	SBI PORTB, 5		
    SBI DDRB, 2   		;PB2 an output 
	CBI DDRC, 2   		;PC2 an input  
AGAIN: 
	SBIS PINC, 2 		;skips the next instruction if PC2 is high 
  	RJMP OVER     		;jump to label OVER 
  	SBI PORTB, 2     
	RJMP AGAIN    		;jump to label AGAIN 
OVER: 
	CBI PORTB, 2  		; make port B2 high 
	LDI R20,5			;set clock prescaler to 1024
	STS TCCR1B,R20		
	LDI R20,0x00		;reset counter to 0
	STS TCNT1H,R20		;reset high bit to 0
	STS TCNT1L,R20		;reset low bit to 0	
delay:
	LDS R29, TCNT1H 	;loading upper bit of counter to R29
	LDS R28, TCNT1L 	;loading lower bit of counter to R28
	CPI R28,0x4A 		;comparing if lower 8 bits of timer is 0x4A
	BRSH body			;if lower bits of timer reached desired amount, 
						;Checking upper bits
	RJMP delay 			;otherwise, keep checking lower bits
body:
	CPI R29,0x4C 		;check to see if upper timer bits have reached the 
						;desired value
	BRLT delay 			;if not, recheck the lower bits
	RET 				;once the timer reached the desired value, toggle the 
						;LED