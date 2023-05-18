;---------------
; Assembly Code
;---------------
#define __SFR_OFFSET 0x00
#include "avr/io.h"
;------------------------
.global start
;------------------------
start:
    LDS   R12, 0x200          ;load direct from data space byte at memory 
                              ;location 0x200 into register
    RET                       ;return to setup() function
;---------------------------------------------------------------------------
