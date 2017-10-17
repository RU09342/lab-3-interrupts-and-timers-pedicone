# Lab 3 - Button Based Delay
#### Jack Pedicone
##### 10-14-2017

## Library Used
msp430.h - default library

## Dependencies
* MSP430 Development boards
* Code Composer Studio
* One LED and one switch

## File name
* delay.c

## Set Requirements
"Setup your microcontroller to initially blink and LED at a rate of 10Hz upon restarting or powering up. Then utilizing one of the buttons on board, a user should be able to set the delay or blinking rate of the LED by holding down a button. The duration in which the button is depressed should then become the new rate at which the LED blinks. As previously stated, you most likely will want to take advantage of the fact that TIMER modules exist and see if you can let them do a bulk of the work for you."

#### Tasks
* [x] Initialize the timer and GPIO pins
* [x] Have the program wait with general interrupts enabled
* [x] Implement a button interrupt that changes the speed of timer
* [x] Implement a timer interrupt that blinks the LED

## Compatibility
* [x] MSP430F5529
* [x] MSP430FR2311
* [x] MSP430FR6989
* [x] MSP430FR5594
* [x] MSP430G2553

### Detailed Description

* Stop watchdog timer
* For FR boards, disable the GPIO power-on default high-impedance mode
* Initialize the timer as an SMCLK (1 MHz) divided by 8 in up mode, that counts to 25000 and then interrupts.
* Initialize GPIO pins for the button and LED
* Enable button interrupt for the button, flip the button edge to high, and clear the interrupt flag
* The edge is flipped so that the pin will detect a change in logic states, which is needed for button interrupts.
* The interrupt flag reports when an interrupt occurs, and must be cleared to prevent an infinite loop.
* Enable GIE to allow general interrupts to occur
* Use pragma vector to begin both the button and timer interrupt
* In the button interrupt, use two if statements of the based on the state of the button interrupt and the button 
* When the button is being held down, flip the edge of the button, stop the first timer and start the second one, and clear the button interrupt flag. The value of the timer of TA1CTL is stored in the register TA1R 
* When the button is released, the value of TA1R is multiplied by 8 and stored in TA0CCR0, which is the number that the timer counts to before interrupting
* The edge is flipped again and the button interrupt flag is cleared again.
* In the timer interrupt, the LED is toggled on and off using an XOR statement, and the IFG for the button is cleared again.

#### Example code

```C
else if(!(P1IES & btn))					// Always runs during a button interrupt
        {
            TA1CTL = MC_0;						// Stop the second timer
            TA0CTL = TASSEL_2 + MC_1 + ID_3;	// Start the first timer as SMCLK divided by 8, in up mode
            TA0CCR0 = TA1R *= 8;				// Take the captured value of the first timer and multiply it by eight
            P1IES &= ~btn;						// Flip High/low edge
            P1IFG &= ~btn						// Clear button interrupt flag
        }  
```

