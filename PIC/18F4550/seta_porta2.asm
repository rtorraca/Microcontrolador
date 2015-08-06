#include P18F4550.INC

CONFIG WDT=OFF; disable watchdog timer
CONFIG MCLRE = ON; MCLEAR Pin on
CONFIG DEBUG = ON; Enable Debug Mode
CONFIG LVP = OFF; Low-Voltage programming disabled (necessary for debugging)
CONFIG FOSC = INTOSCIO_EC;Internal oscillator, port function on RA6
org 0; start code at 0
CBLOCK 0
Delay1:1
Delay2:1
ENDC
ORG 0
;Delay1 res 1 ;reserve 1 byte for the variable Delay1
;Delay2 res 1 ;reserve 1 byte for the variable Delay2
Start:

Main

CLRF LATA,0
CLRF LATB,0
CLRF TRISA
CLRF TRISB

inicio

MOVLW b'00000001'
MOVWF TRISA
MOVWF TRISB

btfsc PORTA,0 ; se for 0 salta, está pressionada
goto testaPRA2

MOVLW b'11011110'
MOVWF PORTB
MOVLW b'00000000'


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

end