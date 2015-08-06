




CLRF PORTA ; Initialize PORTA by
; clearing output
; data latches
CLRF LATA ; Alternate method
; to clear output
; data latches
MOVLW 0Fh ; Configure A/D 
MOVWF ADCON1 ; for digital inputs
MOVLW 07h ; Configure comparators
MOVWF CMCON ; for digital input
MOVLW  0CFh ; Value used to 
; initialize data 
; direction
MOVWF  TRISA ; Set RA<3:0> as inputs
; RA<5:4> as outputs