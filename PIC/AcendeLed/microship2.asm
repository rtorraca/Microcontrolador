CBLOCK	0X70			; POSIÇÃO INICIAL DA RAM

ENDC

#INCLUDE <P16F84A.INC>		; ARQUIVO DE DEFINIÇÕES DO PIC ATUAL


ORG 0x0000

GOTO INICIO


INICIO

BCF STATUS, RP0 ;  Bit Clear STATUS

CLRF PORTA ;Clear PORTA; Initialize PORTA by ; clearing output ; data latches

BSF STATUS, RP0 ;Bit Set STATUS; Select Bank 1

MOVLW 0x0F ; Move literal to W; Value used to; initialize data; direction

MOVWF TRISA ; Set RA<3:0> as inputs; RA4 as output ; TRISA<7:5> are always ; read as ’0’.

END