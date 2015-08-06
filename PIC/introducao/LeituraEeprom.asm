#include "p16f84.inc"
org 0x0
goto Main

org 0x04
goto Main

Main

inicio

bcf   STATUS,RP0   ;bcf ->bit clear em ‘f’. Zera o bit indicado no registro f 
          		   ;Seleciona o banco 0, onde está o registro EEADR 

movlw   30H     ;registro W=constante 30H 

movwf    EEADR  ;move o valor de w para EEADR, EEADR=30H 

bsf   STATUS,RP0   ;bsf -> bit set em ‘f’. Seta o bit indicado no registro f 
          		   ;seleciona o banco 1, onde está o registro EECON1 

bsf    EECON1      ;faz RD=1 iniciando a leitura. 
				   ;A leitura leva apenas um ciclo 

bcf    STATUS,RP0   ;volta ao banco 0, onde está EEDATA 
movf    EEDATA,0    ;W=EEDATA, que é o dado que estava na EEPROM 
movwf    25H      	;escreve W na RAM com endereço 25H. 

goto inicio

End