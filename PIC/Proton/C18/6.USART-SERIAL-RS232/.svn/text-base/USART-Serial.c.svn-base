/****************************************************************
Placa Desenvolvimento PROTO'n PIC18F4550 by Smart Radio
www.SmartRadio.com.br/PIC
Arquivos da placa: www.SmartRadio.com.br/PIC/PROTON
Videos Exemplo: http://www.smartradio.com.br/loja-pic/?id=42

Descrição: Exemplo de comunicação Serial USART RS232,
	       usando a interrupt low priority à cada recepção dado e: 		
			- Ao ligar envia "PROTO'n PIC 18F4550"  e "Init Ok"
 			- Bufferiza até 10 bytes recebidos pela UART e os ecoa de volta pela UART após satisfeita uma de duas condições:
    			1. esperou mais de ~1/2 segundo e não veio mais mada pela serial
				2. encheu o buffer (tamanho definido em: "#define Tamanho_Buffer ... " )
			- Atenção ao baud rate, padrão 9600 8N1 definido na função "Configura_Usart"

Detalhes Importantes sobre a Placa e self programming USB:
	- devido ao Bootloader-self programming USB - ocupar do endereço 0x0000 ao 0x0FFF, o programa/ código 
	deverá começar sempre à partir do endereço 0x1000, para isto usar sempre o linker script "rm18f4550 - HID Bootload.lkr"	
	que fará o realocamento na compilação
	- As interrupções também estão realocadas para os pontos YourHighPriorityISRCode e YourLowPriorityISRCode
	- Devido ao Uso com USB o Clock do PIC está em 48MHz, considerar este quando calculando baud, delays, etc.
	- Configurações de IOs e gerais de hardware estão na função Board_Init()
			
Ligações: C6 TXD/Saída e C7 RXD/Entrada, ligue no seu conversor rs232



REV.0 - FEV2013

Notas: usando programa acima endereço 0x1000 p/ compatibilidade c/ BootLoader USB

*****************************************************************/

#include <p18F4550.h>
#include <delays.h>
#include <usart.h>

// PROTÓTIPOS
void YourHighPriorityISRCode();
void YourLowPriorityISRCode();
void Board_Init (void);
void Configura_Usart (void);
void Configura_TMR1 (void);
void ConsoleTXChar( char c );
void ConsoleTXstr( const char *TxStr );

#define LigaTMR1 T1CONbits.TMR1ON = 1; //liga t1
#define DesligaTMR1 T1CONbits.TMR1ON = 0; //DESLiga t1
#define Tamanho_Buffer 0x0A


// variávei globais
char buffer [Tamanho_Buffer];
int Reg_relogio = 0;
char i = 0;
char temp_i ;

// para usar gravação bootloader por hid
#define PROGRAMMABLE_WITH_USB_HID_BOOTLOADER

	#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)
		#define REMAPPED_RESET_VECTOR_ADDRESS			0x1000
		#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x1008
		#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x1018

	#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)
	extern void _startup (void);        // See c018i.c in your C18 compiler dir
	#pragma code REMAPPED_RESET_VECTOR = REMAPPED_RESET_VECTOR_ADDRESS
	void _reset (void)
	{
	    _asm goto _startup _endasm
	}
	#endif
	#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS
	void Remapped_High_ISR (void)
	{
	     _asm goto YourHighPriorityISRCode _endasm
	}
	#pragma code REMAPPED_LOW_INTERRUPT_VECTOR = REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS
	void Remapped_Low_ISR (void)
	{
	     _asm goto YourLowPriorityISRCode _endasm
	}
	
	#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)
	//Note: If this project is built while one of the bootloaders has
	//been defined, but then the output hex file is not programmed with
	//the bootloader, addresses 0x08 and 0x18 would end up programmed with 0xFFFF.
	//As a result, if an actual interrupt was enabled and occured, the PC would jump
	//to 0x08 (or 0x18) and would begin executing "0xFFFF" (unprogrammed space).  This
	//executes as nop instructions, but the PC would eventually reach the REMAPPED_RESET_VECTOR_ADDRESS
	//(0x1000 or 0x800, depending upon bootloader), and would execute the "goto _startup".  This
	//would effective reset the application.
	
	//To fix this situation, we should always deliberately place a 
	//"goto REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS" at address 0x08, and a
	//"goto REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS" at address 0x18.  When the output
	//hex file of this project is programmed with the bootloader, these sections do not
	//get bootloaded (as they overlap the bootloader space).  If the output hex file is not
	//programmed using the bootloader, then the below goto instructions do get programmed,
	//and the hex file still works like normal.  The below section is only required to fix this
	//scenario.
	#pragma code HIGH_INTERRUPT_VECTOR = 0x08
	void High_ISR (void)
	{
	     _asm goto REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS _endasm
	}
	#pragma code LOW_INTERRUPT_VECTOR = 0x18
	void Low_ISR (void)
	{
	     _asm goto REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS _endasm
	}
	#endif	//end of "#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_LEGACY_CUSTOM_CLASS_BOOTLOADER)"


	#endif

   #pragma code

	
	//These are your actual interrupt handling routines.
	#pragma interrupt YourHighPriorityISRCode
	void YourHighPriorityISRCode()
	{
		//Check which interrupt flag caused the interrupt.
		//Service the interrupt
		//Clear the interrupt flag
		//Etc.
        #if defined(USB_INTERRUPT)
	        USBDeviceTasks();
        #endif
	
	}	//This return will be a "retfie fast", since this is in a #pragma interrupt section 
	#pragma interruptlow YourLowPriorityISRCode
	void YourLowPriorityISRCode()
	{
		//Check which interrupt flag caused the interrupt.
		//Service the interrupt
		//Clear the interrupt flag
		//Etc.
	    char c;
	if ( i == 0 ) LigaTMR1 ; // se 1o byte liga tmr1 pra espera mais
  	c = ReadUSART();
	PIR1bits.RCIF = 0;     /* Clear the interrupt flag */
	buffer [i++] = c; //salva
	Reg_relogio = 0 ; //dá mais tempo pra vir mais
	}	//This return will be a "retfie", since this is in a #pragma interruptlow section 
//
// C O N F I G U R A T I O N   B I T S  

//----------------------------------------------------------------------------
#pragma config PLLDIV   = 5         // (20 MHz crystal on PICDEM FS USB board)
#pragma config CPUDIV   = OSC1_PLL2   
#pragma config USBDIV   = 2         // Clock source from 96MHz PLL/2
#pragma config FOSC     = HSPLL_HS
#pragma config FCMEN    = OFF
#pragma config IESO     = OFF
#pragma config PWRT     = OFF
#pragma config BOR      = ON
#pragma config BORV     = 3
#pragma config VREGEN   = ON      //USB Voltage Regulator
#pragma config WDT      = OFF
#pragma config WDTPS    = 32768
#pragma config MCLRE    = ON
#pragma config LPT1OSC  = OFF
#pragma config PBADEN   = OFF
#pragma config STVREN   = ON
#pragma config LVP      = OFF
#pragma config XINST    = OFF       // Extended Instruction Set 
#pragma config CP0      = OFF
#pragma config CP1      = OFF
#pragma config CPB      = OFF
#pragma config WRT0     = OFF
#pragma config WRT1     = OFF
#pragma config WRTB     = OFF       // Boot Block Write Protection
#pragma config WRTC     = OFF
#pragma config EBTR0    = OFF
#pragma config EBTR1    = OFF
#pragma config EBTRB    = OFF






// Main
void main (void)
{
// Mensagens para serial na RAM
	const char MsgWelcome1 [] = "  PROTO'n PIC 18F4550 \n\r ";

	Board_Init();		// inicia placa
	Configura_Usart ();
	Configura_TMR1 ();

	PORTAbits.RA4 = 1;
	PORTAbits.RA5 = 1;

	// envia pela serial como string (na memória RAM)
	ConsoleTXstr ( MsgWelcome1 );

	// envia pela serial byte à byte um caracter por vez
	ConsoleTXChar ('I');
	ConsoleTXChar ('n');
	ConsoleTXChar ('i');
	ConsoleTXChar ('t');
	ConsoleTXChar (' ');
	ConsoleTXChar ('O');
	ConsoleTXChar ('k');
	ConsoleTXChar ('\r');
	ConsoleTXChar ('\n');	 

 
	  while (1) /* Loop forever */
	{
	
		if (PIR1bits.TMR1IF && i > 0) //se i >0 = recebendo /bufferizando
			{
			PIR1bits.TMR1IF = 0; //limpa flag de estouro T1
			Reg_relogio++;
			if (Reg_relogio == 0x05 || i >= Tamanho_Buffer) 
				{
				temp_i = i;
				for ( temp_i = 0; temp_i < i; temp_i++) //descarregad buffer
					{
					ConsoleTXChar (buffer[temp_i]);
					}
				i = 0;
				Reg_relogio = 0;
				DesligaTMR1  // desliga tmr espera 
				}
			  }
	
	
	} // fecha while loop
	


}



void Board_Init (void)
{
	RCON = 0X80;		// LIMPA REGISTRO DE RESET
	ADCON1 = 0x0F;		// CONFIGURA TODAS AS PORTAS ANALOGICAS COMO I/O Digital

	CMCON = 0x0F;		// ( DESABILITAR COMPARADOR)

    T0CON = 0b11000101;	// timer ativo, 8 bits,clock interno, preescale 1:64

	LATA = 0;
	TRISA = 0b11001111;

	LATB = 0;			// LIMPA O LATCH DA PORTA B
	TRISB = 0;			// tudo Saida
	
	LATC = 0x00;
	TRISC = 0b10111111;
	
	LATD = 0;
	TRISD = 0x00;		// PORTA D TUDO SAIDA  

	LATE = 0;
	TRISE = 0x00;
}

void Configura_Usart (void)
{
  OpenUSART (USART_TX_INT_OFF &
             USART_RX_INT_ON &
             USART_ASYNCH_MODE &
             USART_EIGHT_BIT &
             USART_CONT_RX &
             USART_BRGH_LOW, 77); 	//  clock 48MHz -> 9600 : USART_BRGH_LOW, 77 // 57600 : USART_BRGH_LOW, 12 // 115200 : USART_BRGH_HIGH, 25
  RCONbits.IPEN = 1; /* Enable interrupt priority */
  IPR1bits.RCIP = 0; /* Make receive interrupt LOW priority */
  INTCONbits.GIEH = 1;  /* Enable all high priority interrupts */
  INTCONbits.GIEL = 1;

}

void Configura_TMR1 (void)
{
T1CONbits.TMR1CS = 0; //sinal clock interno
//T1CONbits.TMR1ON = 1; //liga t1
T1CONbits.T1CKPS0 = 1; //preesc 1:8
T1CONbits.T1CKPS1 = 1; // idem
T1CONbits.RD16 = 1; //modo 16 bits
PIR1bits.TMR1IF = 0 ; //limpa flag de estouro
}

void ConsoleTXChar( char c )
{
    while(TXSTAbits.TRMT == 0);
    WriteUSART (c);
}

void ConsoleTXstr( const char *TxStr )
{
	int ii = 0;
	
	do
	{
		ConsoleTXChar( TxStr[ii] );
		ii++;
	}while (TxStr[ii]);
	

}