/*
Button based delay - G2553
Author: Jack Pedicone
 */

#include <msp430.h>
#include <msp430G2553.h>

#define btn 	BIT3;
#define light	BIT0;

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;     				// Stop WDT
	//PM5CTL0 &= ~LOCKLPM5;                   	// Disable the GPIO power-on default high-impedance mode
    TA0CCTL0 = CCIE;                            // CCR0 interrupt enabled
    TA0CTL = TASSEL_2 + MC_1 + ID_3;           	// SMCLK/8, upmode
    TA0CCR0 = 25000;							// Count to 25000 using the timer, then interrupt

    P1DIR |= light & ~btn;            			// LED output, Switch input
    P1REN |= btn;                   			// Enable internal pull-up/down resistors
    P1OUT |= btn;                   			// Select a pull-up resistor for the switch
    P1IE |= btn;                       		// Switch interrupt enabled
    P1IES |= btn;                     			// Toggles between high and low mode for the switch
    P1IFG &= ~btn;                  			// Clears the interrupt flag for the switch

    _BIS_SR(GIE);          						// Enter low power mode with general interrupts enabled

    }

    //Port 1 ISR
    #pragma vector=PORT1_VECTOR					// PORT_1 indicates a button interrupt
    __interrupt void PORT_1(void)
    {
        if(P1IES & btn)							// if both the button interrupt is enabled and the button is held down
        {
        TA0CTL = MC_0;							// Stop the first timer
        TA1CTL = MC_2 + TASSEL_1 + ID_1;		// Start second timer as ACLK divided by 1, in continuous mode
        P1IES &= ~btn;							// Disable/Reenable button interrupt
        P1IFG &= ~btn;							// Clear button interrupt flag
        }
        else if(!(P1IES & btn))				
        {
            TA1CTL = MC_0;						// Stop the second timer
            TA0CTL = TASSEL_2 + MC_1 + ID_3;	// Start the first timer as SMCLK divided by 8, in up mode
            TA0CCR0 = TA1R *= 8;				// Take the captured value of the first timer and multiply it by eight
            P1IES &= ~btn;						// Disable/Reenable button interrupt
            P1IFG &= ~btn						// Clear button interrupt flag
        }
    }

    // Timer A0 interrupt service routine
    #pragma vector=TIMER0_A0_VECTOR
    __interrupt void Timer_A (void)
    {
       P1OUT ^= light;                           // Toggle LED on or off every time it interrupts
       P1IFG &= ~btn;							// Clear button interrupt flag
    }
