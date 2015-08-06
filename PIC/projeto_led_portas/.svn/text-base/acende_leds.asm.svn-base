#include "p16f84.inc"

org 0x0
goto Main

;CT org 0x02


org 0x04
goto Main
Main


;BANK0
;movlw 0xff
;movwf TRISA


BANK1 
movlw 0x00
movwf TRISB


Loop

clrf PORTB
movlw 0x1f
movwf PORTB


goto Loop

End








