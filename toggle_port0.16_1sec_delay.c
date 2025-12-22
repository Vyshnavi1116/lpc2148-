#include <LPC214X.H>

void delay_us(int dlyUS)
{
	// delay function of micro,milli,sec
	dlyUS*=12;
	while(dlyUS--);
}

void delay_ms(int dlyMS)
{
	dlyMS*=12000;
	while(dlyMS--);
}

void delay_s(int dlyS)
{
	dlyS*=12000000;
	while(dlyS--);
}


int main()
{ // delay is added to stable the pins before configuring 
	delay_us(1);
	//set all pins in input mode(default function)
	PINSEL1 = 0;
	//setport0.16 to output mode
	IODIR0 |= 1<<16;
	//non terminating loop 
	while(1)
	{
		//IOSET0 = 1<<16;
		//updating and turn on of portt0.16
		IOPIN0 |= 1<<16;//IOPIN0 = IOPIN0 | (1<<16);
		delay_s(1);
		//IOCLR0 = 1<<16;
		//clears the port0.16
		IOPIN0 &= ~(1<<16);//IOPIN0 = IOPIN0 & ~(1<<16);
		delay_s(1);		
	}
}
