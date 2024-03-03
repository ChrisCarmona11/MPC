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
#include <constantes_int_1.h>

//valores para contadores
int Flag_interrup = 0;
static int contador_ms;
#define LDVAL_TRIGGER_1MS 20640 // 1ms/48.45ns = 20639.83
//regist donde guardar los contadores de ms
static int indexer;			// Variable para ir atravesando el regist

static float scala = 10.0 / 65535.0;

static int regist[128][4];

extern void enable_irq(int irq); // Funci�n para habilitar los regists de interrupci�n

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
	enable_irq(68); // regist 2 y 4 de sobra asi que el bit flag ser� el 4 creo.
	// Enable timer and interrupt
	PIT_TCTRL0 = (PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK);
}

float toVolts(int number) {
	float valor = number * scala;
	return valor;
}

void pit0_isr(void) {
	PIT_TFLG0 |= PIT_TFLG_TIF_MASK; //Clear LPT Compare flag
	indexer = 0;
	for (;indexer<4;indexer++) {
		regist[contador_ms][indexer] = tabla[contador_ms][indexer];
	}
	contador_ms++; //Aumentar el contador de milisegundos
	Flag_interrup= 1;
	printf ("%d\n",&regist);
}

int main(void) {

	//Configuracion de los timers
	pit_interrupt_config();

	int max[4] = { 0, 0, 0, 0 };
	int min[4] = { 0, 0, 0, 0 };
	int accumulative[4] = { 0, 0, 0, 0 };
	max[3]=17;
	int j = 0;
	for (;;) {
		if (Flag_interrup == 1) {
			for (j = 0; j < 4; j++) {
				if (regist[contador_ms][j] > max[j]) {
					max[j] = regist[contador_ms][j];
				}
				if (regist[contador_ms][j] < min[j]) {
					min[j] = regist[contador_ms][j];
				}
				accumulative[j] += regist[contador_ms][j];
			}
			printf ("%d\n",&regist);
			if (contador_ms == 128) {
				memset(regist, 0, sizeof(int) * 128 * 4);
				float promedio[4] = { };
				for (j = 0; j < 4; j++) {
					promedio[j] = ((float) accumulative[j]) / 128.0;
				}
				printf("Maximos: [%6i,%6i,%6i,%6i] \n", max[0], max[1], max[2],
						max[3]);
				printf("Minimos: [%6i,%6i,%6i,%6i] \n", min[0], min[1], min[2],
						min[3]);

				for (j = 0; j < 4; j++) {
					max[j] = 0;
					min[j] = 0;
					accumulative[j] = 0;
					promedio[j] = 0;
				}
				contador_ms = 0;

			}
			Flag_interrup = 0;
		}
	}

	return 0;
}

