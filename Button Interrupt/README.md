# Lab 3 - Button Interrupt
#### Jack Pedicone
##### 10-14-2017

## Library Used
msp430.h - default library

## Dependencies
* MSP430 Development boards
* Code Composer Studio
* One LED and one switch

## File name
* interrupt.c

## Set Requirements
"Your goal for this part of the lab is to replicate your button code from Lab 2, where the LED should change states only when the button is pressed. This can be extended to include behaviors such as only have the LED on when the button is depressed, or have the LED blink one color when pressed and another when it is let go. Another behavior extends from the second lab which is speed control based on the button presses. For example, have the rate of the LED cycle between a "low", "Medium", and "High" rate of speed."

#### Tasks
* [x] Initialize the timer and GPIO pins
* [x] Have the program wait with general interrupts enabled
* [x] Implement a button interrupt that detects when the state of the button changes

## Compatibility
* [x] MSP430F5529
* [x] MSP430FR2311
* [x] MSP430FR6989
* [x] MSP430FR5594
* [x] MSP430G2553

### Detailed Description

* Stop watchdog timer
* For FR boards, disable the GPIO power-on default high-impedance mode
* Initialize GPIO pins for the button and LED
* Enable button interrupt for the button, flip the button edge to high, and clear the interrupt flag
* The edge is flipped so that the pin will detect a change in logic states, which is needed for button interrupts.
* The interrupt flag reports when an interrupt occurs, and must be cleared to prevent an infinite loop.
* Enable GIE to allow general interrupts to occur
* Use pragma vector to begin the button interrupt
* When the button is pressed, it leaves the main function and enters the button interrupt section
* In the button interrupt, use an XOR statement to toggle the LED on or off depending on the state, then clear the interrupt flag for the button 


#### Example code from FR5994

```C
//Port 1 ISR
    #pragma vector=PORT1_VECTOR					// PORT_1 indicates a button interrupt
    __interrupt void PORT_1(void)
{
  P5OUT ^= light;                            // Toggle LED on/off
  PIFG &= ~btn;                           // Button IFG cleared
}

```

# Extra Work

#### Multiple buttons
Come up with a behavior of your own that incorporates needing to use two buttons or more and these two buttons must be implemented using interrupts.

* Done on the FR5994
* Define 2 LEDs and two buttons, the buttons have edges flipped and flags cleared
* In the button interrupt, include two if statements that check which button is being pressed
* The LED that is toggled on or off depends on the button being held
* That particular button's flag is cleared

#### Example Code
```C
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

```

