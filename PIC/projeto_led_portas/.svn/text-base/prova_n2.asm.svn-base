#include "p16f84.inc"

org 0x0
goto Main

;CT org 0x02


org 0x04
goto Main

 

Main

	bsf STATUS,RP0;seta RP0 do status, Bank 1
	bcf STATUS,RP1;seta RP1 do status
	movf TRISA,0xff
	clrf TRISB
       
    

	bcf STATUS,RP0;seta RP0 do status
	bcf STATUS,RP1;seta RP1 do status
	clrf PORTA    
	clrf PORTB
	movlw 0xff 
	movwf PORTA ;manda 1 para a PORTA
   
inicio

bcf PORTA,0

btfsc PORTA,0x00 ; se for 0 salta, está pressionada
goto testaPRA2
movf PORTB,B'00001001'

goto inicio


testaPRA2
bcf PORTA,1
btfsc PORTA,0 ; se for 0 salta, está pressionada

goto limpa

movf PORTB,B'00000011'

goto Main

limpa 
bcf PORTA,0
clrf PORTB

goto inicio

End