/*
 * main.c
 *
 *  Created on: Feb 27, 2024
 *      Author: chris
 */
/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */
#include <stdio.h>
#include <string.h>
#include "Driver_SLCD.h"

//valores para contadores

#define VALOR_OBJETIVO 999
#define LDVAL_TRIGGER_1MS 20640 // 1ms/48.45ns = 20639.83
//Registro donde guardar los contadores de ms
#define RegistroSize 10
static int registro[RegistroSize];

static int LPTMR_INTERRUPT; // Flag de detteccion de interrupcion de segundo
static int int_counter;		// Contador de segundos
static int contador_ms;		// Contador de milisegundos
static int indexer;			// Variable para ir atravesando el registro

//lo que ocupa un numero en la pantalla LCD

#define CharWidth 6

//Los numeros de la pantalla 

static int unidades, decenas, centenas;
static int regDecenas, regCentenas;

extern void enable_irq(int irq); // Funci�n para habilitar los registros de interrupci�n

void lptmr_clear_registers() {
	LPTMR0_CSR = 0x00;
	LPTMR0_PSR = 0x00;
	LPTMR0_CMR = 0x00;
}

void lptmr_interrupt() {
	int compare_value = 999;  //value must be less than 0xFFFF

	//Enable Port clocks
	SIM_SCGC5 |=
			SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK
					| SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

	/* Enable LPT Module */SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;

	LPTMR_INTERRUPT = 0; //Clear global variable

	//Reset LPTMR module
	lptmr_clear_registers();

	/* Enable LPT Interrupt in NVIC*/
	enable_irq(85); //LPTMR Vector is 101. IRQ# is 101-16=85

	/* Configure LPT */LPTMR0_CMR = LPTMR_CMR_COMPARE(compare_value); //Set compare value
	LPTMR0_PSR = LPTMR_PSR_PCS(0x1) | LPTMR_PSR_PBYP_MASK; //Use LPO clock and bypass prescale

	// Habilita la interrupcion del timer.
	LPTMR0_CSR = LPTMR_CSR_TIE_MASK;  //Enable LPT interrupt

	LPTMR0_CSR |= LPTMR_CSR_TEN_MASK; //Turn on LPTMR and start counting

}

void lptmr_isr(void) {
	LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;  	//Clear LPT Compare flag
	LPTMR_INTERRUPT = 1;  				//Set global variable
	int_counter++; 						//Aumentar variable de segundo
	registro[indexer] = contador_ms;	//Guardar el contador de milisegundos
	contador_ms = 0;					//Limpiar el constador de milisegundos
}

void pit_interrupt_config(void)
/*
 * Contador para detectar la interrupci�n de milisegundo mediante un preescaler
 * */
{
	// Enable the clock to the PIT module
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
	// Enable PIT module control register
	PIT_MCR = PIT_MCR_FRZ_MASK;
	// Disable timer and interrupt
	PIT_TCTRL0 = 0;
	// Load value (Timer Load Value Register (PIT_LDVALn))
	PIT_LDVAL0 = LDVAL_TRIGGER_1MS; // Trigger 1 ms
	// Clear any pending interrupt
	PIT_TFLG0 |= PIT_TFLG_TIF_MASK;
	// Enable interrupt
	enable_irq(68); // Registro 2 y 4 de sobra asi que el bit flag ser� el 4 creo.
	// Enable timer and interrupt
	PIT_TCTRL0 = (PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK);
}

void pit0_isr(void) {
	PIT_TFLG0 |= PIT_TFLG_TIF_MASK; //Clear LPT Compare flag
	contador_ms++;//Aumentar el contador de milisegundos
}

void prettyPrint(int number) {
	unidades = number % 10;
	decenas = (number / 10) % 10;
	centenas = (number / 100) % 10;

	_SLCDModule_PrintNumber(unidades, CharWidth * 5);
	if (regDecenas != decenas) {
		_SLCDModule_PrintNumber(decenas, CharWidth*4);
		regDecenas= decenas;
	}
	if (regCentenas != centenas) {
		_SLCDModule_PrintNumber(centenas, CharWidth*3);
		regCentenas=centenas;
	}

}

int main(void) {

	LPTMR_INTERRUPT = 0;					//Inicializar el Flag a cero

	//Configuracion de los timers
	lptmr_interrupt();
	pit_interrupt_config();

	//Iniciamos el modulo para encender la pantalla
	_SLCDModule_Init();
//	_SLCDModule_PrintString("",0);
	
	for (;;) {

		if (LPTMR_INTERRUPT == 1) {
			LPTMR_INTERRUPT = 0;	//Limpiar el Flag de la inerrupt de segundo
			indexer++;		//Pasar a la siguiente posici�n para escribir los ms
			prettyPrint(int_counter);

		}

		if (indexer == RegistroSize) {//La siguiente posici�n ser� out or range
			indexer = 0;

		}
	}
	_SLCDModule_TurnOffAllSegments();
	_SLCDModule_TurnOffBattery();

	return 0;
}

