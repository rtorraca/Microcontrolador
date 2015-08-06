#include <p18f4550.h>
#include <delays.h>

// The lower 4 bits of this port will be used
#define LCD_DATA		LATB
#define LCD_EN 			LATBbits.LATB4
#define LCD_RS 			LATBbits.LATB5

#define TRIS_LCD_DATA	TRISB
#define TRIS_LCD_EN 	TRISBbits.TRISB4
#define TRIS_LCD_RS 	TRISBbits.TRISB5

// Commands for Hitachi LCD
#define CLEAR_DISPLAY	0x01
#define FOUR_BIT   	0b00101111  /* 4-bit Interface               */
#define EIGHT_BIT  	0b00111111  /* 8-bit Interface               */
#define LINE_5X7   	0b00110011  /* 5x7 characters, single line   */
#define LINE_5X10  	0b00110111  /* 5x10 characters               */
#define LINES_5X7  	0b00111011  /* 5x7 characters, multiple line */

#define DISPLAY_ON  0b00001111  /* Display on      */
#define DISPLAY_OFF 0b00001011  /* Display off     */
#define CURSOR_ON   0b00001111  /* Cursor on       */
#define CURSOR_OFF  0b00001101  /* Cursor off      */
#define BLINK_ON    0b00001111  /* Cursor Blink    */
#define BLINK_OFF   0b00001110  /* Cursor No Blink */
#define UNDERLINE_ON    0b00001111
#define UNDERLINE_OFF   0b00001101

#define INCREMENT	0b00000111	/* Entry mode increment */
#define DECREMENT	0b00000101	/* Entry mode decrement */
#define SHIFT_ON	0b00000111	/* Display shift on		*/
#define SHIFT_OFF	0b00000110	/* Display shift off	*/

unsigned char ii;
#define PulseEnable { LCD_EN = 1; for (ii = 0; ii < 10; ii++); LCD_EN = 0; }

void SmartDelay5ms(void) ;
void SmartDelay15ms(void);
void SmartLCDInit(void);
void SmartLCDCmd(unsigned char);
void SmartLCDChar(unsigned char);



// TODO: Add all prototypes

void SmartDelay5ms(void) {
	Delay1KTCYx(60); // Delay of 5ms
	// Cycles = (TimeDelay * Fosc) / 4
	// Cycles = (5ms * 8MHz) / 4
	// Cycles = 10,000
	return;
}
//
void SmartDelay15ms(void) {
	Delay1KTCYx(180); // Delay of 15ms
	// Cycles = (TimeDelay * Fosc) / 4
	// Cycles = (15ms * 8MHz) / 4
	// Cycles = 30,000
	return;
}
//
void SmartLCDInit(void) {
	// Set port directions
	TRIS_LCD_DATA &= 0xf0;	// Clear lower 4 bits
	TRIS_LCD_EN = 0;
 	TRIS_LCD_RS = 0;

	SmartLCDCmd(FOUR_BIT);			// Nigel's code sends this before the next command?
	SmartLCDCmd(FOUR_BIT & LINES_5X7);
//	SmartDelay15ms();
	SmartLCDCmd(INCREMENT & SHIFT_OFF);
//	SmartDelay15ms();
	SmartLCDCmd(DISPLAY_ON & BLINK_OFF & UNDERLINE_OFF);
//	SmartDelay15ms();
	SmartLCDCmd(CLEAR_DISPLAY);
//	SmartDelay15ms();
}
//
void SmartLCDCmd(unsigned char c) {
	LCD_RS = 0;

	LCD_DATA &= 0xf0;
	LCD_DATA |= (c >> 4);
	PulseEnable;

	LCD_DATA &= 0xf0;
	LCD_DATA |= (c & 0x0f);
	PulseEnable;

	SmartDelay5ms();
	LCD_DATA &= 0xf0;
}
//
void SmartLCDStr ( const char *Texto_LCD )
{
	int i = 0;
	
	do
	{
		SmartLCDChar ( Texto_LCD[i] );
		i++;
	}while (Texto_LCD[i]);

}
//
void SmartLCDChar(unsigned char c) {
	LCD_DATA &= 0xf0;
	LCD_DATA |= (c >> 4);
	LCD_RS = 1;
	PulseEnable;
	LCD_RS = 0;

	LCD_DATA &= 0xf0;
	LCD_DATA |= (c & 0x0f);
	LCD_RS = 1;
	PulseEnable;
	LCD_RS = 0;
	SmartDelay5ms();
	LCD_DATA &= 0xf0;

}
//
void Inicio_1Linha (void)
{
	SmartLCDCmd (0x80);
}
//
void Inicio_2Linha (void)
{
SmartLCDCmd (0xC0);
}

