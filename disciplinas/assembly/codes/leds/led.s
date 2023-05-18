.global main			;defining a global function
	.type main, @function 	;defining the type

main:
	ldi r16,0b00100000	;ldi loads an immediate into registers 16 through 31.
	ldi r17,0b00000000
	; The “register” DDRB (Section 14.4 of the manual), which is at address 0x4,
	; configures the pins of port B are for input (0) or output (1).

	; out loads to the I/O address the value that is stored in the register
	
	; Loading 0b00000001 to address 0x4, making pin 5 of Port B used as output,
	; and the others as input
	out 0x04,r16		

LOOP:
	; Register PORTB (address 0x5) defines whether a high or low signal is sent
	; to the output pins of port B. Let's send logic 1 (5 volts) into PB5.
	out 0x05,r17
	jmp LOOP		; Jump to loop label (infinite loop)
