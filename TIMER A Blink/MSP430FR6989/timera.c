/*
Button based delay - FR6989
Author: Jack Pedicone
credit to Josh Gould for the initTimer function
 */
 
#include <msp430.h>

#define light   BIT0;
#define light2  BIT7;

void initTimer (int freq); //captures desired frequency

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode

  initTimer(20); // Initialize timer at 10Hz or 0.1s

  P1DIR |= light;                           // The two LEDs are set as outputs
  P9DIR |= light2;
  P1OUT &= ~light;                          // Start with both LEDs off
  P9OUT &= ~light2;
  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt

}

void initTimer (int freq)
{
    CCTL0 = CCIE;                   // CCR0 interrupt enabled
    CCTL1 = CCIE;
    TA0CTL = TASSEL_2 + MC_1 + ID_3;  // SMCLK/8 in upmode
    TA1CTL = TASSEL_2 + MC_1 + ID_3;  // SMCLK/8 in upmode
    int capture = (125000) / freq;    // manage frequency
    TA0CCR0 = capture;                // (1000000/8)/(12500) = 10 Hz = 0.1 seconds
    TA1CCR0 = capture *= 2;
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
   P1OUT ^= light;                          // Toggle the first LED on and off
}

// Timer A1 interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer_B (void)
{
   P9OUT ^= light2;                          // Toggle the second LED on and off
}
