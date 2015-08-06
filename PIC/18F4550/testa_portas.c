#include <htc.h>

__CONFIG(1,0x0F24);
__CONFIG(2,0X0000);


void Wait()
{
   unsigned char i;
   for(i=0;i<100;i++)
      _delay(60000);
}


void main()
{
   //Initialize PORTD
   //PD0 as Output
   TRISD=0b11111110;

   //Now loop forever blinking the LED.
   while(1)
   {
      LATD=0B00000001;  //PORTD0 = HIGH

      Wait();

      LATD=0B00000000;  //PORTD0 = LOW
      Wait();
   }
}
