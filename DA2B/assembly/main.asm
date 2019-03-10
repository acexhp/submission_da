; assembly.asm
; Author : hierholz


.include <m328pdef.inc>
.ORG 0						;location for reset
JMP MAIN
.ORG 0x02					;location for external interrupt 0
JMP EX0_ISR

MAIN:
	LDI R20, HIGH(RAMEND)
	OUT SPH, R20
	LDI R20, LOW(RAMEND)
	LDI R20, 0X2			;initialize stack
	LDI R20, 0X2			;make INT0 falling edge triggered
	STS EICRA, R20			
	SBI DDRB, 5				;set PORTB5 as output
	SBI PORTB, 5			;turns off led
	SBI PORTD, 2			;pull-up activated
	LDI R20, 1<<INT0		;enable INT0
	OUT EIMSK, R20			;bit masking
	SEI						;enable interrupts

HERE: JMP HERE	

EX0_ISR:
	IN R21, PORTB			;read PORTB as input
	LDI R22, (1<<5)			;00100000 for toggling PB5
	EOR R21, R22			;exor to toggle led
	OUT PORTB, R21			;led turns on
	RCALL delay_1s
	RCALL delay_250ms
	RETI					;returns from interrupt 


delay_1s:					;creating delay of 1s
	rcall delay_100ms
	rcall delay_100ms
	rcall delay_100ms
	rcall delay_100ms
	rcall delay_100ms
	rcall delay_100ms
	rcall delay_100ms
	rcall delay_100ms
	rcall delay_100ms
	rcall delay_100ms
	ret

delay_250ms:				;creating delay of 250ms
	rcall delay_100ms
	rcall delay_100ms
	rcall delay_50ms
	ret

delay_100ms:				;creating delay of 100ms
	rcall delay_10ms
	rcall delay_10ms
	rcall delay_10ms
	rcall delay_10ms
	rcall delay_10ms
	rcall delay_10ms
	rcall delay_10ms
	rcall delay_10ms
	rcall delay_10ms
	rcall delay_10ms
	ret
delay_50ms:					;creating delay of 50ms
	rcall delay_10ms
	rcall delay_10ms
	rcall delay_10ms
	rcall delay_10ms
	rcall delay_10ms

delay_10ms:					;creating delay of 10ms
	rcall delay_1ms
	rcall delay_1ms
	rcall delay_1ms
	rcall delay_1ms
	rcall delay_1ms
	rcall delay_1ms
	rcall delay_1ms
	rcall delay_1ms
	rcall delay_1ms
	rcall delay_1ms
	ret

delay_1ms:					;creating delay of 1ms
	rcall delays_16MHz
	rcall delays_16MHz
	rcall delays_16MHz
	rcall delays_16MHz
	rcall delays_16MHz
	rcall delays_16MHz
	rcall delays_16MHz
	rcall delays_16MHz
	ret

delays_16MHz:				;delays 1ms for 16MHz
	push r16				;save value in r16
	ldi  r16, 180			;running 1980 cycles
delays_16MHz_:				;1ms per loop	
	nop						;1 cycle
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	dec  r16
	brne delays_16MHz_		;2 cycles
	pop  r16				;restore value in r16
	ret
