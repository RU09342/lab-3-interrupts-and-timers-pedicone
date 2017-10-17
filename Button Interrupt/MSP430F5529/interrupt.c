/*
Button interrupt - F5529
Author: Jack Pedicone
 */

#include <msp430.h>

#define light 	BIT0;
#define btn 	BIT1;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  //PM5CTL0 &= ~LOCKLPM5;                   	// Disable the GPIO power-on default high-impedance mode
  P1DIR |= light;                            // Set LED to output direction
  
  P2IE |=  btn;                            // Button interrupt enabled
  P2IES |= btn;                            // Switch high/low edge flipped
  P2REN |= btn;                            // Enable Pull Up on switch
  P2IFG &= ~btn;                           // Button IFG cleared

  __bis_SR_register(LPM4_bits + GIE);       // Enter LPM4 w/interrupt
}

//Port 1 ISR
    #pragma vector=PORT1_VECTOR					// PORT_1 indicates a button interrupt
    __interrupt void PORT_1(void)
{
  P1OUT ^= light;                            // Toggle LED on/off
  P2IFG &= ~btn;                           // Button IFG cleared
}

