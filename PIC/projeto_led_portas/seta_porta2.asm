#include "p16f84.inc"

org 0x00
goto Main

org 0x04
goto Main


Main

    clrf PORTA
    bsf STATUS,RP0;seta RP0 do status
    clrf TRISA
    bcf STATUS,RP0

    clrf PORTB
    bsf STATUS,RP0;seta RP0 do status
    clrf TRISB
    bcf STATUS,RP0

	movlw 0xff 
	movwf PORTA ;manda 1 para a PORTA
	

;	bcf PORTA,4
;	bcf PORTA,3
repete
 
	btfsc PORTA,4 ;testa se o bit RA4 está está em 0 
        		   ;se estiver salta ativa led

	goto limpaprb7 

	bsf PORTB,7    ;se tiver acende bit RB7
	goto testab3

limpaprb7 bcf PORTB,7

	testab3
   
 	btfsc PORTA,3 ;testa se o bit RA3 está está em 0 
        		   ;se estiver salta ativa led
	goto limpaprb6 
    bsf PORTB,6    ;se tiver acende bit RB6

goto repete

limpaprb6 bcf PORTB,6

goto repete

    End





