/****************************************************************
Placa Desenvolvimento PROTO'n PIC18F4550 by Smart Radio
www.SmartRadio.com.br/PIC
Arquivos da placa: www.SmartRadio.com.br/PIC/PROTON
Videos Exemplo: http://www.smartradio.com.br/loja-pic/?id=42

Descrição: Sequencia LEDs em todos IOs da placa

Detalhes Importantes sobre a Placa e self programming USB:
	- devido ao Bootloader-self programming USB - ocupar do endereço 0x0000 ao 0x0FFF, o programa/ código 
	deverá começar sempre à partir do endereço 0x1000, para isto usar sempre o linker script "rm18f4550 - HID Bootload.lkr"	
	que fará o realocamento na compilação
	- As interrupções também estão realocadas para os pontos YourHighPriorityISRCode e YourLowPriorityISRCode
	- Devido ao Uso com USB o Clock do PIC está em 48MHz, considerar este quando calculando baud, delays, etc.
	- Configurações de IOs e gerais de hardware estão na função Board_Init()

Ligações: LEDs externos conectados nos IOs



REV.0 - FEV2013

Notas: usando programa acima endereço 0x1000 p/ compatibilidade c/ BootLoader USB

*****************************************************************/

#include <p18F4550.h>
#include <delays.h>


void YourHighPriorityISRCode();
void YourLowPriorityISRCode();
void Board_Init (void);
void Delay_LED (void);

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



void main (void)
{
	Board_Init();		// inicia placa

	PORTA = 0xFF;		// acende todos - teste
	PORTB = 0xFF;
	PORTC = 0xFF;
	PORTD = 0xFF;
	PORTE = 0xFF;
	Delay_LED ();
	PORTA = 0x00;		// apaga todos
	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
	PORTE = 0x00;
	Delay_LED ();


	while (1)
	{

		PORTAbits.RA0 = 1;		// Ativa LED
		Delay_LED ();			// Aguarda tempo aceso
		PORTAbits.RA0 = 0;		// apaga LED
		PORTAbits.RA1 = 1;		// e repete ...
		PORTAbits.RA0 = 0;		// apaga LED
		Delay_LED ();			// 
		PORTAbits.RA1 = 0;		// 
		PORTAbits.RA2 = 1;		// e repete ...
		Delay_LED ();			// 
		PORTAbits.RA2 = 0;		// 
		PORTAbits.RA3 = 1;		// e repete ...
		Delay_LED ();			// 
		PORTAbits.RA3 = 0;		// 
		PORTAbits.RA4 = 1;		// e repete ...
		Delay_LED ();			// 
		PORTAbits.RA4 = 0;		// 
		PORTAbits.RA5 = 1;		// e repete ...
		Delay_LED ();			// 
		PORTAbits.RA5 = 0;		// 

		PORTEbits.RE0 = 1;		// e repete ... PORT E
		Delay_LED ();			// 
		PORTEbits.RE0 = 0;		// 
		PORTEbits.RE1 = 1;		// e repete ...
		Delay_LED ();			// 
		PORTEbits.RE1 = 0;		// 
		PORTEbits.RE2 = 1;		// e repete ...
		Delay_LED ();			// 
		PORTEbits.RE2 = 0;		// 

		PORTCbits.RC0 = 1;		// e repete ... PORT C 
		Delay_LED ();			// 
		PORTCbits.RC0 = 0;		// 
		PORTCbits.RC1 = 1;		// e repete ...
		Delay_LED ();			// 
		PORTCbits.RC1 = 0;		// 
		PORTCbits.RC2 = 1;		// e repete ...
		Delay_LED ();			// 
		PORTCbits.RC2 = 0;		// 
		PORTCbits.RC6 = 1;		// e repete ...
		Delay_LED ();			// 
		PORTCbits.RC6 = 0;		// 
		PORTCbits.RC7 = 1;		// e repete ...
		Delay_LED ();			// 
		PORTCbits.RC7 = 0;		// 

		PORTDbits.RD0 = 1;		// e repete ... PORT D
		Delay_LED ();			// 
		PORTDbits.RD0 = 0;		// 
		PORTDbits.RD1 = 1;		// e repete ... PORT D
		Delay_LED ();			// 
		PORTDbits.RD1 = 0;		// 
		PORTDbits.RD2 = 1;		// e repete ... PORT D
		Delay_LED ();			// 
		PORTDbits.RD2 = 0;		// 
		PORTDbits.RD3 = 1;		// e repete ... PORT D
		Delay_LED ();			// 
		PORTDbits.RD3 = 0;		// 
		PORTDbits.RD4 = 1;		// e repete ... PORT D
		Delay_LED ();			// 
		PORTDbits.RD4 = 0;		// 
		PORTDbits.RD5 = 1;		// e repete ... PORT D
		Delay_LED ();			// 
		PORTDbits.RD5 = 0;		// 
		PORTDbits.RD6 = 1;		// e repete ... PORT D
		Delay_LED ();			// 
		PORTDbits.RD6 = 0;		// 
		PORTDbits.RD7 = 1;		// e repete ... PORT D
		Delay_LED ();			// 
		PORTDbits.RD7 = 0;		// 

		PORTBbits.RB0 = 1;		// e repete ... PORT B
		Delay_LED ();			// 
		PORTBbits.RB0 = 0;		// 
		PORTBbits.RB1 = 1;		// e repete ... PORT B
		Delay_LED ();			// 
		PORTBbits.RB1 = 0;		// 
		PORTBbits.RB2 = 1;		// e repete ... PORT B
		Delay_LED ();			// 
		PORTBbits.RB2 = 0;		// 
		PORTBbits.RB3 = 1;		// e repete ... PORT B
		Delay_LED ();			// 
		PORTBbits.RB3 = 0;		// 
		PORTBbits.RB4 = 1;		// e repete ... PORT B
		Delay_LED ();			// 
		PORTBbits.RB4 = 0;		// 
		PORTBbits.RB5 = 1;		// e repete ... PORT B
		Delay_LED ();			// 
		PORTBbits.RB5 = 0;		// 
		PORTBbits.RB6 = 1;		// e repete ... PORT B
		Delay_LED ();			// 
		PORTBbits.RB6 = 0;		// 
		PORTBbits.RB7 = 1;		// e repete ... PORT B
		Delay_LED ();			// 
		PORTBbits.RB7 = 0;		// 

	}


}

void Delay_LED (void)
{
		Delay10KTCYx(40);
}

void Board_Init (void)
{
	RCON = 0X80;				// LIMPA REGISTRO DE RESET
	ADCON1 = 0x0F;				// CONFIGURA TODAS AS PORTAS ANALOGICAS COMO I/O 

	CMCON = 0x0F;				// DESABILITA COMPARADOR

    T0CON = 0b11000101;			// timer ativo, 8 bits,clock interno, preescale 1:64

	TRISA = 0b00000000;
	TRISB = 0b00000000;			// todos Saida
	TRISC = 0b00000000;
	TRISD = 0b00000000;		
	TRISE = 0b00000000;

	PORTA = 0x00;				// nível 0 saída
	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
	PORTE = 0x00;

}