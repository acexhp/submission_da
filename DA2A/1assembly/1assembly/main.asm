;
; 1assembly.asm
; Author : hierholz
;


.org 0
	LDI R16,4			;representing PB2
	OUT DDRB, R16		;enable PB2 as output
	LDI R17,0			;resets PB2
	LDI R20,5			;set clock prescaler to 1024
	STS TCCR1B,R20		
begin:
	LDI R20,0x00		;reset counter to 0
	STS TCNT1H,R20		;reset high bit to 0
	STS TCNT1L,R20		;reset low bit to 0
	RCALL delay			;calls label delay
	OUT PORTB,R17		;LED turns on for 435ms
	EOR R17,R16			;XOR to toggle led
	LDI R20,0x00		;resetting the counter to 0
	STS TCNT1H,R20		;resetting high bit to 0
	STS TCNT1L,R20		;resetting low bit to 0
	RCALL delay2		;calls label delay2
	OUT PORTB,R17		;LED turns on for 290ms
	EOR R17,R16			;XOR to toggle led
	RJMP begin 			;repeat main loop
delay:
	LDS R29, TCNT1H 	;loading upper bit of counter to R29
	LDS R28, TCNT1L 	;loading lower bit of counter to R28
	CPI R28,0x8C 		;comparing if lower 8 bits of timer is 0x8C
	BRSH body			;if lower bits of timer reached desired amount, 
						;Checking upper bits
	RJMP delay 			;otherwise, keep checking lower bits
body:
	CPI R29,0x1A 		;check to see if upper timer bits have reached the 
						;desired value
	BRLT delay 			;if not, recheck the lower bits
	RET 				;once the timer reached the desired value, toggle the 
						;LED

delay2:
 	LDS R29, TCNT1H 	;loading upper bit of counter to R29
	LDS R28, TCNT1L 		;loading lower bit of counter to R28
 	CPI R28,0xB2 		;comparing if lower 8 bits of timer is 0xB2
 	BRSH body2 			;if lower bits of timer have reached desired amount, 
						;check the upper bits
 	RJMP delay2 		;otherwise, keep checking lower bits
body2:
 	CPI R29,0x11 		;check to see if upper timer bits have reached the 
						;desired value
 	BRLT delay2 		;if not, recheck the lower bits
 	RET 				;once the timer reached the desired value, toggle the 
						;LED
