# Lab 3 - Timer A
#### Jack Pedicone
##### 10-14-2017

## Library Used
msp430.h - default library

## Dependencies
* MSP430 Development boards
* Code Composer Studio
* Two LEDs

## File name
* timera.c

## Set Requirements
Using the TIMER module instead of a software loop, control the speed of two LEDS blinking on your development boards. Experiment with the different counting modes available as well as the effect of the pre-dividers. Why would you ever want to use a pre-divider? What about the Capture and Compare registers? Your code should include a function (if you want, place it in its own .c and .h files) which can convert a desired Hz into the proper values required to operate the TIMER modules.

#### Tasks
* [x] Implement a function that will allow an entered frequency in (hertz) to work with the timers
* [x] Create two timer interrupts that each control the state of an individual LED.

## Compatibility
* [x] MSP430F5529
* [x] MSP430FR2311
* [x] MSP430FR6989
* [x] MSP430FR5594
* [x] MSP430G2553

### Detailed Description

* * Create the void initTimer before the main
* Stop watchdog timer
* For FR boards, disable the GPIO power-on default high-impedance mode
* Initialize GPIO pins for the LEDs
* Enable GIE to allow general interrupts to occur
* Inside the initTimer function, enable the capture/compare register for both timers
* Set both timers to use the SMCLK (1MHz) divided by 8 (ID_3) in upmode
* Divide 125000 (the frequency of the clock after being divided by 8) by the entered freq (in the code it's 20), and set that to the integer 'capture'.
* Set the compare/control register of the first timer to capture, which in this case is 10 Hz
* Set the compare/control register of the second timer to double the value of capture, which in this case is the equivalent of 5 Hz
* Use pragma vector to begin the timer interrupts
* In both timer interrupts, use an XOR statement to toggle the LED on or off depending on the state, with each timer controlling a different LED. 


#### Example code from FR6989

```C
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

```

