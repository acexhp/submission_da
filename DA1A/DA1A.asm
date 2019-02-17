;Student Name: Allis Hierholzer
;Student #: 2000160356
;Student E-mail: hierholz@unlv.nevada.edu
;Primary Github Address: 
;Directory: 

.org 0					;Giving a memory place to start

	.SET m1 = 0xFF		;Sets value for variable m1 -> highest 8bits of multiplicand
	.SET m2 = 0xFF		;Sets value for variable m2 -> lowest 8bits of multiplicand
	.SET m3 = 0xFF		;Sets value for variable m3 -> 8bits multiplier
 
	LDI R25, m1			;R25 = m1 
	LDI R24, m2			;R24 = m2 
	LDI R22, m3			;R22 = m3
	CLR R26				;Clears R26 to R30 
	CLR R27
	CLR R28
	CLR R29
	CLR R30

Loop:					;Multiplies 16bits x 8bits using iterative addition
	ADD R18, R24		;R18 = R18 + R24
	ADC R19, R25		;R19 = R19 + R25 + carry 
	ADC R20, R26		;R20 = R20 + R26 + carry
	DEC R22				;Decrements R22 by 1
	BRNE Loop			;Branches to label Loop until R22 = 0

Verify:					;Verifies 16bits x 8bits using the MUL instruction
	LDI R22, m3			;R22 = m3
	MUL R24, R22		;R24 x R22 (product automatically stored in R0 & R1)
	MOV R27, R0			;Moves values in R0 to R27
	MOV R28, R1			;Moves values in R1 to R28
	MUL R25, R22		;R25 x R22 (product automatically stored in R0 & R1)
	MOV R29, R1			;Moves values in R0 to R29
	ADD R28, R0			;R28 = R28 + R0
	ADC R29, R30		;R29 = R29 + R30 + carry
	
						;At the end of "Verify" label,
						;R18 should be the same as R27
						;R19 should be the same as R28
						;R20 should be the same as R29

Done:
	JMP Done			;Program ends
	

