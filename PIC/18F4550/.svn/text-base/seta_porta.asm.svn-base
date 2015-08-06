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
CLRF PORTD
CLRF TRISD
CLRF Delay1
CLRF Delay2
MainLoop:
BTG PORTD,RD1 ;Toggle PORT D PIN 1 (20)
Delay:
DECFSZ Delay1,1 ;Decrement Delay1 by 1, skip next instruction if Delay1 is 0
GOTO Delay
DECFSZ Delay2,1
GOTO Delay
GOTO MainLoop
end