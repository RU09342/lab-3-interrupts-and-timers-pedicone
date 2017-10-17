/*
Button interrupt - FR2311
Author: Jack Pedicone
 */

#include <msp430.h>

#define light 	BIT0;
#define btn 	BIT1;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  PM5CTL0 &= ~LOCKLPM5;                   	// Disable the GPIO power-on default high-impedance mode
  P1DIR |= light;                            // Set LED to output direction
  P1IE |=  btn;                            // Button interrupt enabled
  P1IES |= btn;                            // Switch high/low edge flipped
  P1REN |= btn;                            // Enable Pull Up on switch
  P1IFG &= ~btn;                           // Button IFG cleared

  __bis_SR_register(LPM4_bits + GIE);       // Enter LPM4 w/interrupt
}

// Port 1 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void)
#else
#error Compiler not supported!
#endif
{
  P1OUT ^= light;                            // Toggle LED on/off
  P1IFG &= ~btn;                           // Button IFG cleared
}
