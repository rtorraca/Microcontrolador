#include <p18F4550.inc>
	
	CONFIG WDT=OFF; disable watchdog timer
	CONFIG MCLRE = ON; MCLEAR Pin on
	CONFIG DEBUG = ON; Enable Debug Mode
	CONFIG LVP = OFF; Low-Voltage programming disabled (necessary for debugging)
	CONFIG FOSC = INTOSCIO_EC;Internal oscillator, port function on RA6 

org 0x0
goto Main

;CT org 0x02


org 0x04
goto Main

 

Main

  
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