;---------------
; Assembly Code
;---------------
#define __SFR_OFFSET 0x00
#include "avr/io.h"
;------------------------
.global start
;------------------------
start:
    LDI   R16, 0xFF           ;load immediate byte 0xFF into register
;    LDI   R15, 0xFF           ;trying to use a different register
    RET                       ;return to setup() function
;---------------------------------------------------------------------------
