;---------------
; Assembly Code
;---------------
#define __SFR_OFFSET 0x00
#include "avr/io.h"
;------------------------
.global start
.global btnLED
;------------------------
start:
    SBI   DDRB, 5             ;set PB5 (13) as output
    CBI   DDRD, 2             ;set PD2 (2) as input
    RET                       ;return to setup() function
;---------------------------------------------------------------------------
btnLED:
ledON:
    ;SBIC = Skip if Bit in Register is Cleared
    ;SBIS = Skip if Bit in Register is Set
    SBIC  PIND, 2             ;skip next statement if button not pressed
    RJMP  ledOFF              ;jump to ledOFF
    SBI   PORTB, 5            ;set D19 to high
    RJMP  ledON               ;return to label ledON
ledOFF:
    CBI   PORTB, 5            ;turn OFF LED if button is not pressed
    RET
