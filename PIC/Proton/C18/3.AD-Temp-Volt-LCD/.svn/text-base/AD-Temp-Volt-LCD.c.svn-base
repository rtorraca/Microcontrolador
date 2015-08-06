/****************************************************************
Placa Desenvolvimento PROTO'n PIC18F4550 by Smart Radio
www.SmartRadio.com.br/PIC
Arquivos da placa: www.SmartRadio.com.br/PIC/PROTON
Videos Exemplo: http://www.smartradio.com.br/loja-pic/?id=42

Descrição: Mostra no LCD temperatura (entrada AN0 ) e tensão 0-5Volts (entrada AN1)

Detalhes Importantes sobre a Placa e self programming USB:
	- devido ao Bootloader-self programming USB - ocupar do endereço 0x0000 ao 0x0FFF, o programa/ código 
	deverá começar sempre à partir do endereço 0x1000, para isto usar sempre o linker script "rm18f4550 - HID Bootload.lkr"	
	que fará o realocamento na compilação
	- As interrupções também estão realocadas para os pontos YourHighPriorityISRCode e YourLowPriorityISRCode
	- Devido ao Uso com USB o Clock do PIC está em 48MHz, considerar este quando calculando baud, delays, etc.
	- Configurações de IOs e gerais de hardware estão na função Board_Init()

Ligações: Sensor de temperatura LM35 conectado ao AN0 da placa, tensão 0-5Volts no An1 e LCD 
- LM35 no AN0:
 ____ ___
|     | -\------- Gnd
|     | --|----Vout------AN0
|_____|__/------- +5V



- tensão variável 0-5V no AN1


- LCD no PORTB:
Pino Placa		Pino LCD		
RB0 ------------ 11 (DB4)
RB1 ------------ 12 (DB5)
RB2 ------------ 13 (DB6)
RB3 ------------ 14 (DB7)
RB4 ------------ 6  (EN)
RB5 ------------ 4  (RS)
		GND----+ 5  (RW)
			   + 1  (Gnd)  
		+5V----- 2  (Vdd)
Trimpot--------- 3  (Vo)		



REV.0 - FEV2013

Notas: usando programa acima endereço 0x1000 p/ compatibilidade c/ BootLoader USB

*****************************************************************/

#include <p18F4550.h>
#include <delays.h>
#include <adc.h>

void YourHighPriorityISRCode();
void YourLowPriorityISRCode();
void Board_Init (void);
void Delay_LED (void);
void AlternaLEDS (void);
void SmartLCDInit(void);
void SmartLCDCmd(unsigned char);
void SmartLCDChar( unsigned char);
void SmartLCDStr ( const char *Texto_LCD ); 
void Inicio_2Linha (void);
void Inicio_1Linha (void);
void Inicio_2Linha (void);
int LE_ADC (const int CANAL_AD);

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
	
	}	//This return will be a "retfie", since this is in a #pragma interruptlow section 

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

// Mensagens na RAM
const char MsgWelcome1 [] = "  Conversor A/D ";
const char MsgWelcome2 [] = "  Temp./Tensao  ";

const char MsgWelcome3 [] = "    Aguarde,   ";
const char MsgWelcome4 [] = "  Iniciando...  ";

const char MsgTemp [] = " Temp. : ";
const char MsgTens [] = " Tensao: ";
const char MsgGC [] =   "oC      ";
const char MsgAP [] =   "Vdc     ";

void main (void)
{

	int i;
	char saida[13] ;
	int VALOR_AD;
	int CANAL_1 = 0x01;

	long TENSAO;

	char Tensao_T [5] = "0000" ;
	char Tensao_V [5] = "0000" ;

	Board_Init();		// inicia placa

	SmartLCDInit();		// Inicializa LCD
	
	Inicio_1Linha ();
	SmartLCDStr ( MsgWelcome3 );

	Inicio_2Linha ();
	SmartLCDStr ( MsgWelcome4 );
	
	PORTAbits.RA4 = 1;

	for (i=0;i<13;i++)
		AlternaLEDS();

	Inicio_1Linha ();
	SmartLCDStr ( MsgWelcome1 );
 	
	Inicio_2Linha ();
	SmartLCDStr ( MsgWelcome2 );

	for (i=0;i<13;i++)
		AlternaLEDS();	

	while (1)
	{
		Inicio_1Linha ();
		SmartLCDStr ( MsgTemp );
		
		// executa leitura AD canal 0 = sensor temp LM35Dz
		VALOR_AD = LE_ADC ( 0 ); 
		// Converte o valor lido de tipo int p/ char-string p/ exibição no LCD
		TENSAO = (((long)VALOR_AD*5000 )/1023);
		Tensao_T [0] = (( TENSAO / 100 ) ) + 0x30;
		Tensao_T [1] = (( TENSAO / 10 ) % 10) + 0x30;
		Tensao_T [2] = '.';
		Tensao_T [3] = (( TENSAO / 1 ) % 10) + 0x30;

		Inicio_1Linha ();
		SmartLCDStr ( MsgTemp );	// 1a linha escreve "temp: "
		SmartLCDStr ( Tensao_T );	// escreve a tensão lida
		SmartLCDStr ( MsgGC );		// escreve "oC"

		// executa leitura AD canal 1 = tensão 0-5V
		VALOR_AD = LE_ADC ( CANAL_1 ); // vai ler o AD canal 1/AN1 = tensão
		// Converte o valor lido de tipo int p/ char-string p/ exibição no LCD	
		TENSAO = (((long)VALOR_AD*5000 )/1023);
		Tensao_V [0] = (  TENSAO / 1000 ) + 0x30;
		Tensao_V [1] = '.';
		Tensao_V [2] = (( TENSAO / 100 )  % 10) + 0x30;
		Tensao_V [3] = (( TENSAO / 10 ) % 10) + 0x30;

		Inicio_2Linha ();			// 2a linha
		SmartLCDStr ( MsgTens );	// escreve "Tensão"
		SmartLCDStr ( Tensao_V );	// e a tensão lida
		SmartLCDStr ( MsgAP );		// seguida de "Vdc"-> MsgAP

		Nop();

		AlternaLEDS();	

	}


}
//
int LE_ADC (const int CANAL_AD)
{
	int Valor_AD;

	// seleciona Canal / pino entrada PIC
	if (CANAL_AD == 1)
		ADCON0bits.CHS0 = 1; 
	else
		ADCON0bits.CHS0 = 0; 

  	Delay10TCYx(1);     		// Delay por 10TCY

  	ConvertADC();         		// Start conversion
 	while( BusyADC() );   		// Wait for completion
  	Valor_AD = ReadADC();   	// Read result

	return Valor_AD;

}
//
void AlternaLEDS (void)
{

		PORTAbits.RA4 ^= 1;
		PORTAbits.RA5 ^= 1;
		Delay10KTCYx (100);
}
//
void Board_Init (void)
{
	RCON = 0X80;				// LIMPA REGISTRO DE RESET
	ADCON1 = 0x0D;				// CONFIGURA AS PORTAS ANALOGICAS AN0 e AN1 como Analog. restante digitais

	CMCON = 0x0F;				// DESABILITA COMPARADOR

    T0CON = 0b11000101;			// timer ativo, 8 bits,clock interno, preescale 1:64

	TRISA = 0b00000011;			// como entradas AN0 e 1 
	TRISB = 0b00000000;			// todos Saida
	TRISC = 0b00000000;
	TRISD = 0b00000000;		
	TRISE = 0b00000000;

	PORTA = 0x00;				// nível 0 saída
	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
	PORTE = 0x00;


	// inicializa AD e habilita
	ADCON0 = 0b00000001 ;
	ADCON2 = 0b10111110 ;		
  	Delay10TCYx( 5 );     // Delay for 50TCY

}