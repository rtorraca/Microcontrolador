#include "p16f84.inc"


org 0x00
goto Main


org 0x08
goto Main


Main

;RP0 em 1 bco 1 apenas 
;RP0 em 0 bco 0


bsf STATUS,RP0
bcf STATUS,RP1

movlw 0x0f
movwf 0x8c

movlw 0xff
movwf 0x8c


bsf STATUS,RP0
bcf STATUS,RP1

movlw 0x00
movwf TRISA


bcf STATUS,RP0
bcf STATUS,RP1

movlw 0xff
movwf 0x05



goto Main

End