/****************************************************************
Placa Desenvolvimento PROTO'n PIC18F4550 by Smart Radio
www.SmartRadio.com.br/PIC
Arquivos da placa: www.SmartRadio.com.br/PIC/PROTON
Videos Exemplo: http://www.smartradio.com.br/loja-pic/?id=42

Descrição: - Controle de Servo por interrupção, até 3 servos, valores de controle 0-250 nas variáveis globais
			i0(servo RB0), i1(servo RB1) e i2(servo RB2), 
			- Uma ótima referência de como servos funcionam (em inglês):  http://www.servocity.com/html/how_do_servos_work_.html
			- Imagens - diagramas servos na pasta programa /em other files

Detalhes Importantes sobre a Placa e self programming USB:
	- devido ao Bootloader-self programming USB - ocupar do endereço 0x0000 ao 0x0FFF, o programa/ código 
	deverá começar sempre à partir do endereço 0x1000, para isto usar sempre o linker script "rm18f4550 - HID Bootload.lkr"	
	que fará o realocamento na compilação
	- As interrupções também estão realocadas para os pontos YourHighPriorityISRCode e YourLowPriorityISRCode
	- Devido ao Uso com USB o Clock do PIC está em 48MHz, considerar este quando calculando baud, delays, etc.
	- Configurações de IOs e gerais de hardware estão na função Board_Init()

Ligações: até 3 Servos pinos RB0, RB1e RB2

observação: servos podem consumir mais de 500mA que, é a corrente máxima teórica de portas USB portanto, caso suspeite que a Usb
            não fornecerá corrente suficiente utilize uma fonte externa (de 5volts) adequada p/ alimentação dos servos, 
			o regulador da placa também suporta 100mA no máximo.


REV.0 - FEV2013

Notas: usando programa acima endereço 0x1000 p/ compatibilidade c/ BootLoader USB

*****************************************************************/

#include <p18F4550.h>
#include <delays.h>
#include "Interrupts.h"  			// header files


/** P R O T Ó T I P O S */
void Timer0_Init(void);

void YourHighPriorityISRCode();
void YourLowPriorityISRCode();
void Board_Init (void);
void Delay_LED (void);
void AlternaLEDS (void);

// variáveis contendo valores posição servos
int i0 = 1;
int i1 = 127;
int i2 = 250;


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

		int i= 251;

        #if defined(USB_INTERRUPT)
	        USBDeviceTasks();
        #endif

	
	    if  (INTCONbits.TMR0IF)				// Check for Timer0 Interrupt
	    {
			PORTBbits.RB0 = 1;				// inverte estado do PORTD0 (LED0)
			PORTBbits.RB1 = 1;				// inverte estado do PORTD0 (LED0)
			PORTBbits.RB2 = 1;				// inverte estado do PORTD0 (LED0)	
	
	        INTCONbits.TMR0IF = 0;			// clear (reset) flag  
			TMR0H = 0x15;    				// preset for Timer0 MSB register
			TMR0L = 0xA0;    				// preset for Timer0 LSB register
	
			Delay100TCYx(120);					// pulso de 1mS que é mínimo
	
			do
			{
				if  ( i0 == i )
					PORTBbits.RB0 = 0;			
		
				if  ( i1 == i )
					PORTBbits.RB1 = 0;			
		
				if  ( i2 == i )
					PORTBbits.RB2 = 0;			
			} while ( i-- );
	
	    }

		// aqui loop incremento só pra demonstração incrementa sozinho
		i0++;
		if ( i0 == 251 )
			i0 = 0;			
	
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

unsigned int Servo_Position = 0;


void main (void)
{
	int i;


	Board_Init();		// inicia placa

	Timer0_Init();

	PORTAbits.RA4 = 1;

	for (i=0;i<13;i++)
		AlternaLEDS();
	

	while (1)
	{


		Nop();

		AlternaLEDS();	

	}


}
//
void AlternaLEDS (void)
{

		PORTAbits.RA4 ^= 1;
		PORTAbits.RA5 ^= 1;
		Delay10KTCYx (10);
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

	// Interrupts
	INTCON2bits.RBPU = 0; 	// habilita pull ups portB
	INTCON2bits.RBIP = 0;	// Interrupção portB = baixa prioridade

//	INTCONbits.RBIE = 1;	// habilita interrupt na mudança estado portB 4 à 7

	RCONbits.IPEN = 1; 		// habilita interrupts c/ níveis de prioridades

	INTCONbits.RBIF = 0;	// limpa Flag deinterrupt

	INTCONbits.GIEL = 1;	// habilita interrupts baixa prioridade
	INTCONbits.GIEH = 1;	// habilita interrupts alta prioridade


}
//
void Timer0_Init(void)
{
    // Set up Interrupts for timer
    INTCONbits.TMR0IF = 0;          // clear roll-over interrupt flag
	INTCON2bits.TMR0IP = 1;			// Interrupção Timer 0 = Alta prioridade
    INTCONbits.TMR0IE = 1;          // enable the Timer0 interrupt.
    // Set up timer itself
   	T0CON = 0b00000001;             // prescale 1:xx 
	TMR0H =  0x15;    				// preset for Timer0 MSB register
	TMR0L = 0xA0;    				// preset for Timer0 LSB register
    T0CONbits.TMR0ON = 1;           // start timer
}
//
