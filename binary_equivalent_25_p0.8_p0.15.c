 #include <lpc214x.h>
/*void delay_s(int dlyS)
{
	dlyS*=12000000;
	while(dlyS--);
}*/
#include"delay.h"
int main(void)
{
    unsigned int value = 25;            // Decimal number to display
    value = value << 8;                 // Shift so LSB starts at P0.8

    /* Configure P0.8 – P0.15 as GPIO */
    PINSEL0 = 0x00000000;               // Ensure pins are GPIO
    PINSEL1 = 0x00000000;

    /* Set P0.8 – P0.15 as output */
    IODIR0 |= (0xFF << 8);              // 8 bits from P0.8 to P0.15
    IOCLR0 = (0xFF << 8);           // Clear the output region
    IOSET0 = value;                 // Output binary of 25 on P0.8–P0.15
		while(1);
   
	}
  
