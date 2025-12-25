/*************************************************************
 * File Name  : mixed_active_high_low_binary_display.c
 * Purpose    : Display a binary count (0–255) on LEDs connected 
 *              to P0.8–P0.15 where lower 4 LEDs are active-high 
 *              and upper 4 LEDs are active-low.
 * Controller : LPC2148
 *************************************************************/

#include <lpc214x.h>     // Header file for LPC214x series register definitions



/*
 ----------------------------------------------------------------------------------------
| Parameter          | Value             | Description                                   |
| ------------------ | ----------------- | --------------------------------------------- |
| System Clock       | 60 MHz            | MCU executes 60 million cycles per second     |
| One CPU cycle      | 16.67 ns          | Time per instruction cycle                    |
| Loop iteration     | ~5 cycles         | Cost for each decrement and branch iteration  |
| delay(1) duration  | ~1 second         | Based on ~12,000,000 loop iterations          |
 ----------------------------------------------------------------------------------------

Loop Instruction Cost Breakdown:
  - 1 cycle  ? decrement
  - 1 cycle  ? comparison
  - 3 cycles ? branching overhead
----------------------------------------------
˜ Total ~5 cycles per loop
*/


/*--------------------------------------------------------------------
 Delay Function (Time-wasting software loop)
 ---------------------------------------------------------------------
  - Does not use timers; consumes CPU cycles to create delay
  - Input parameter determines delay duration
 --------------------------------------------------------------------*/
void delay(int dly_s)
{
    dly_s *= 12000000;   // Scaled loop count for approx. 1-second delay
    while(dly_s--);      // Busy-wait execution loop
}



int main()
{
    unsigned int count = 0, out;   // Count variable and processed LED output value



/*--------------------------------------------------------------------
GPIO INPUT CONFIGURATION: P0.3 for Push Button
----------------------------------------------------------------------
PINSEL0 ? Selects function mode for pins P0.0–P0.15
 - Bits [7:6] control P0.3 mode
 - Clearing bits ensures GPIO mode
--------------------------------------------------------------------*/
    PINSEL0 &= ~(3 << 3);    // Configure P0.3 as GPIO


/*--------------------------------------------------------------------
IODIR0 ? Configures pin direction
 - '0' ? INPUT
Here:
 (1 << 3) clears direction bit ? P0.3 becomes input (switch)
--------------------------------------------------------------------*/
    IODIR0 &= ~(1 << 3);     // Set P0.3 as input



/*--------------------------------------------------------------------
GPIO OUTPUT CONFIGURATION FOR LEDs: P0.8–P0.15
----------------------------------------------------------------------
PINSEL1 ? Controls alternate functions for P0.16–P0.31,
but clearing lower 16 bits ensures GPIO routing for P0.8–P0.15.
--------------------------------------------------------------------*/
    PINSEL2 &= ~0x0000FFFF;  // Ensure pins remain as GPIO


/*--------------------------------------------------------------------
IODIR0 configuration for pins P0.8–P0.15
 - Binary mask: 1111 1111 0000 0000 ? OUTPUT
--------------------------------------------------------------------*/
    IODIR1 |= 0x00FF0000;    // Configure P0.8–P0.15 as output


/*--------------------------------------------------------------------
Initialize LEDs
----------------------------------------------------------------------
Value:
    0x0000F000 ? sets only upper nibble of LEDs HIGH initially
Meaning:
    LEDs connected to bits P0.12–P0.15 start ON depending on polarity
--------------------------------------------------------------------*/
    IOSET1 = 0x00F00000;



/*--------------------------------------------------------------------
Main Program Loop
----------------------------------------------------------------------
 - Waits for button press (active-low logic)
 - Increments count
 - Adjusts output for mixed active-high / active-low LEDs
 - Displays binary pattern on LED group P0.8–P0.15
--------------------------------------------------------------------*/
    while(1)
    {

        /*---------------------------------------------------------------
        Button Check:
        Active-low ? Switch pressed when logic reads '0'
        ---------------------------------------------------------------*/
        if(!(IOPIN0 & (1 << 3)))
        {
            count++;     // Increment binary counter value


            /*-----------------------------------------------------------
            Prepare LED output:

             out = count;          ? store raw counter
             out ^= 0xF0;          ? invert upper 4 bits 
                                     (because LEDs connected here are active-LOW)

            Example:
              count = 0b10101010
              Upper nibble inverted:
                1010 ? 0101
              Final out reflects this mixed LED behavior.
            -----------------------------------------------------------*/
            out = count;
            out ^= 0xF0;     // Invert upper nibble for active-low LED configuration


            /*-----------------------------------------------------------
            Output data to P0.8–P0.15

             Step 1: Clear previous LED values
             Step 2: Shift result into bit positions 8–15
            -----------------------------------------------------------*/
            IOCLR1 = 0x00FF0000;     // Clear only targeted LED pins
            IOSET1 = (out << 16);     // Display processed binary pattern


            /*-----------------------------------------------------------
            Wait until button is released
            Prevents auto-repeat (multiple increments during long press)
            -----------------------------------------------------------*/
            while(!(IOPIN0 & (1 << 3)));    // Hold until switch released
						delay(1);
        }
    }
}