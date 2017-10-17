/*
Button interrupt - FR5994
Author: Jack Pedicone
 */

#include <msp430.h>

#define light   BIT0;
#define light2  BIT1;
#define btn     BIT6;
#define btn2    BIT5;


int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode
  P1DIR |= light;                            // Set LED to output direction
  P1DIR |= light2;                            // Set LED to output direction

  P5IE |=  btn;                            // Button interrupt enabled
  P5IES |= btn;                            // Switch high/low edge flipped
  P5REN |= btn;                            // Enable Pull Up on switch
  P5IFG &= ~btn;                           // Button IFG cleared

  P5IE |=  btn2;                            // Button interrupt enabled
  P5IES |= btn2;                            // Switch high/low edge flipped
  P5REN |= btn2;                            // Enable Pull Up on switch
  P5IFG &= ~btn2;                           // Button IFG cleared

  __bis_SR_register(GIE);       // Enter LPM4 w/interrupt
}

//Port 5 ISR
    #pragma vector=PORT5_VECTOR     // PORT_5 indicates a button interrupt
    __interrupt void PORT_5(void)
{
        if(P5IE & btn)				//Checks if the first button is being held down
        {
            P5OUT ^= light;         // Toggle LED on/off
            P5IFG &= ~btn;          // Button IFG cleared
        }
        if(P5IE & btn2)				//Checks if the second button is being held down
        {
            P5OUT ^= light2;        // Toggle LED 2 on/off
            P5IFG &= ~btn2;         // Button 2 IFG cleared
        }
        P5IFG=0;

}
