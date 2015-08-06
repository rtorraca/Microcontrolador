#include "p16f84.inc"



;_CONFIG _CP_OFF &_PWRTE_ON &_XT_OSC


org 0x00
goto Main

org 0x04
goto Main

Main

;BANK0

bcf STATUS,RP0; seta algo em 0

clrf PORTA; limpa a porta

bsf STATUS,RP0; "Setar" o bit RP0 do registro STATUS

movlw 0xff;  W recebe 1 todos bits sao de entrada

movwf TRISA; recebe o W tudo entrada 11111111

bcf STATUS,RP0; RP0 de status em 0

clrf PORTB; limpa a porta

movlw 0xff; w recebe todos em 11111111

movwf TRISB; recebe w para jogar 1 logico nas saidas


Loop

movlw 0xff; w recebe todos em 11111111

movwf PORTB; recebe w para jogar 1 logico nas saidas


movlw 0x00; w recebe todos em 11111111



goto Loop

End








