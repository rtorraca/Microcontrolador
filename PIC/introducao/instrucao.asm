#include "p16f84.inc"
org 0x0
goto Main
;CT org 0x02

org 0x04
goto Main

Main

inicio

bcf STATUS,RP0;define o bank 0, em RP0=0, bit 5 STATUS
movlw 0xff
movlw 0x0f


bsf STATUS,RP0; define o bank 1, em RP0=1, bit 5 STATUS
movlw 0x0f
movlw 0xff


clrf INDF
;goto Loop

movlw 0x0c ;definição do inicio do endereço
movwf FSR ; FSR aponta p/ o endereço 0x0C

LOOP clrf INDF ; INDF = 0
incf FSR; endereço = endereço inicial + 1
btfss FSR,4 ; todos os locais de memoria limpos?
goto LOOP ; não, para o loop de novo
goto inicio

End

