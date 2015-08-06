CBLOCK	0X70			; POSIÇÃO INICIAL DA RAM

ENDC

#INCLUDE <PIC16F84A.INC>		; ARQUIVO DE DEFINIÇÕES DO PIC ATUAL


org 0x00
goto Main

org 0x04
goto Main



Main 

BANK0


movlw 0x00 ; w recebe  o valor literal 0x00

acres

addlw 0x1; adiciona literal a w
movwf PORTB ; manda para a porta B o q estava em w

Loop goto acres

end
