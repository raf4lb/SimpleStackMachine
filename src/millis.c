/*___________________________________________________________________________________________________

Title:
	millis.c v2.1
	
	For complete details visit
	https://www.programming-electronics-diy.xyz/2021/01/millis-and-micros-library-for-avr.html

Author:
 	Liviu Istrate
	istrateliviu24@yahoo.com
	www.programming-electronics-diy.xyz

Donate:
	Software development takes time and effort so if you find this useful consider a small donation at:
	paypal.me/alientransducer
_____________________________________________________________________________________________________*/


/* ----------------------------- LICENSE - GNU GPL v3 -----------------------------------------------

* This license must be included in any redistribution.

* Copyright (C) 2021 Liviu Istrate, www.programming-electronics-diy.xyz (istrateliviu24@yahoo.com)

* Project URL: https://www.programming-electronics-diy.xyz/2021/01/millis-and-micros-library-for-avr.html

* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program. If not, see <https://www.gnu.org/licenses/>.

--------------------------------- END OF LICENSE --------------------------------------------------*/

#include "millis.h"

/*====================================================================================================
	FUNCTIONS
======================================================================================================*/

/*----------------------------------------------------------------------------------------------------
	Timer configuration.
	
	f_cpu				CPU clock frequency in Hertz. When RTC is used, the clock is
						always 32768 Hz.
------------------------------------------------------------------------------------------------------*/
void millis_init(uint32_t f_cpu){
	#if AVR_CLASS == 0
		// Timer settings
		SET_TCCRA();
		SET_TCCRB();
		REG_TIMSK = (1 << BIT_OCIE);
		REG_OCR = ((F_CPU / PRESCALER) / 1000) - 1; // 1000 is the frequency
		
	#else
	
		#if MILLIS_TIMING_MODULE == TIMING_USING_TCA
			TCA0.SINGLE.PER = f_cpu / 1000 + MILLIS_ERR_CAL;		// set period to 1ms
			// Set prescaler to 1 for 16-bit timers
			//TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc;		// set prescaler to 1
			TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP0_bm;				// enable Compare Channel 0 Interrupt
			TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;				// enable timer TCA
		
		#elif MILLIS_TIMING_MODULE == TIMING_USING_RTC
			// The RTC peripheral is used internally during device start-up. 			// Always check the Busy bits in the RTC.STATUS and 
			// RTC.PITSTATUS registers, also on initial configuration.
			while(RTC.STATUS > 0);									// wait for all register to be synchronized
			
			// Configure the Clock CLK_RTC.
			// System clock (CLK_PER) is required to be at least 			// four times faster than RTC clock (CLK_RTC) for			// reading counter value, and this is regardless of the RTC_PRESC setting.
			// Timeout period: (2^16 / 32768) = 2s
			// Resolution: (2^16 / 32768) / 2^16 * 10^6 = 30.5us
			// The CLK_RTC clock configuration is used by both RTC and PIT functionality.
			RTC.CLKSEL = MILLIS_RTC_CLOCK;
			while(RTC.STATUS > 0);									// wait for all register to be synchronized (CMP Busy flag)
			// Set period (TOP) value (1000us / 30.5us) - 1
			//RTC.PER = (1000.0 / ((65536.0 / f_cpu) / 65536.0 * 1000000.0)) + MILLIS_ERR_CAL;
			RTC.PER = 32 + MILLIS_ERR_CAL;
			RTC.INTCTRL |= RTC_OVF_bm;								// Overflow Interrupt Enable
			while(RTC.STATUS > 0);									// wait for all register to be synchronized (CTRLA Busy flag)
			RTC.CTRLA |= RTC_RTCEN_bm;								// enable RTC
		#endif
	
	#endif
	
	sei();	// enable Global Interrupts
}


/*----------------------------------------------------------------------------------------------------
	Returns current milliseconds
------------------------------------------------------------------------------------------------------*/
millis_t millis_get(){
	millis_t ms;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		ms = milliseconds;
	}
	
	return ms;
}


/*----------------------------------------------------------------------------------------------------
	Turn on timer and resume time keeping
------------------------------------------------------------------------------------------------------*/
void millis_resume(){
	#if AVR_CLASS == 0
		pwr_enable();
		SET_TCCRB();
		REG_TIMSK |= (1 << BIT_OCIE);
	#else
		#if MILLIS_TIMING_MODULE == TIMING_USING_TCA
			TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm; // enable TCA timer
			
		#elif MILLIS_TIMING_MODULE == TIMING_USING_RTC
			RTC.CTRLA |= RTC_RTCEN_bm; // enable RTC
		#endif
	#endif
}


/*----------------------------------------------------------------------------------------------------
	Pause time keeping and turn off timer to save power
------------------------------------------------------------------------------------------------------*/
void millis_pause(){
	#if AVR_CLASS == 0
		REG_TIMSK &= ~(1 << BIT_OCIE);
		REG_TCCRB = 0;
		pwr_disable();
	#else
		#if MILLIS_TIMING_MODULE == TIMING_USING_TCA
			TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm; // disable TCA timer
			
		#elif MILLIS_TIMING_MODULE == TIMING_USING_RTC
			// Wait for all register to be synchronized (CTRLA Busy flag)
			while(RTC.STATUS > 0);
			RTC.CTRLA &= ~RTC_RTCEN_bm; // disable RTC
		#endif
	#endif
}


/*----------------------------------------------------------------------------------------------------
	Reset milliseconds count to 0
------------------------------------------------------------------------------------------------------*/
void millis_reset(){
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		milliseconds = 0;
	}
}


/*----------------------------------------------------------------------------------------------------
	Add time
------------------------------------------------------------------------------------------------------*/
void millis_add(millis_t ms){
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		milliseconds += ms;
	}
}


/*----------------------------------------------------------------------------------------------------
	Subtract time
------------------------------------------------------------------------------------------------------*/
void millis_subtract(millis_t ms){
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		milliseconds -= ms;
	}
}


/*----------------------------------------------------------------------------------------------------
	Attach a custom function to timer interrupt
------------------------------------------------------------------------------------------------------*/
void millis_interrupt_attach(void (*funcPtr)(void)){
	interruptCallback = funcPtr;
}



/*----------------------------------------------------------------------------------------------------
	ISR Handlers
------------------------------------------------------------------------------------------------------*/
#if AVR_CLASS == 0	// non-UPDI
	ISR(ISR_VECT){
	
#else				// UPDI

	#if MILLIS_TIMING_MODULE == TIMING_USING_TCA
		ISR(TCA0_CMP0_vect){
	#elif MILLIS_TIMING_MODULE == TIMING_USING_RTC
		ISR(RTC_CNT_vect){
	#endif
	
		#if MILLIS_TIMING_MODULE == TIMING_USING_TCA
			TCA0.SINGLE.INTFLAGS = TCA_SINGLE_CMP0_bm;	// clear channel 0 compare flag
		#elif MILLIS_TIMING_MODULE == TIMING_USING_RTC
			//RTC.INTFLAGS = RTC_CMP_bm;				// clear compare flag
			RTC.INTFLAGS = RTC_OVF_bm;					// clear overflow flag
		#endif
#endif
		
	++milliseconds;
	
	if(interruptCallback){
		interruptCallback();	
	}
}