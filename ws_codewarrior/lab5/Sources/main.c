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
#include <float.h>
#include <math.h>

//valores para contadores
int Flag_interrup = 0;
static int contador_ms = 0;
#define LDVAL_TRIGGER_1MS 20640 // 1ms/48.45ns = 20639.83
//regist donde guardar los contadores de ms
static int canal_int,canal;			// Variable para ir atravesando el regist

static float scala = 10.0 / 65535.0;

static int regist[128][4];
static int buffer[128][4];

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
	for (canal_int = 0; canal_int < 4; canal_int++) {
		buffer[contador_ms][canal_int]= tabla[contador_ms][canal_int];
	}
	contador_ms++; //Aumentar el contador de milisegundos
	if (contador_ms == 125){ 
		Flag_interrup = 0;}
	if (contador_ms == 128) {
		Flag_interrup = 1;
		contador_ms = 0;
	}
}

int main(void) {

	//Configuracion de los timers
	pit_interrupt_config();

	int max[4] = { 0, 0, 0, 0 };
	int min[4] = { 0, 0, 0, 0 };
	int accumulative[4] = { 0, 0, 0, 0 };
	float quadratic[4] = { 0, 0, 0, 0 };
	float max_f[4];
	float min_f[4];
	canal = 0;
	int muestra = 0;
	while (1) {
		if (Flag_interrup == 1) {
			memcpy(regist, buffer, sizeof(int) * 128 * 4); 	// volcar al registro la muesta entera
			memset(buffer, 0, sizeof(int) * 128 * 4); 			// Limpiar el buffer
			for (canal = 0; canal < 4; canal++) {
				for (muestra = 0; muestra < 128; muestra++) {
					if (regist[muestra][canal] > max[canal]) {
						max[canal] = regist[muestra][canal];
					}
					if (regist[muestra][canal] < min[canal]) {
						min[canal] = regist[muestra][canal];
					}
					accumulative[canal] += regist[muestra][canal];
					quadratic[canal] +=  toVolts(regist[muestra][canal])
							* toVolts(regist[muestra][canal]);
					if ( muestra == 127) 
						Flag_interrup= 0;
				}
			}
			float promedio[4];
			float promedioQ[4];

			for (canal = 0; canal < 4; canal++) {
				max_f[canal] = toVolts(max[canal]);
				min_f[canal] = toVolts(min[canal]);
				promedio[canal] = toVolts(
						(int) (((float) accumulative[canal]) / 128.0));
				promedioQ[canal] = sqrt(quadratic[canal] / 128.0);

				//Limpiar los registros de las variables que se visualizan.
				max[canal] = 0;
				min[canal] = 0;
				accumulative[canal] = 0;
				quadratic[canal] = 0;
			}
			
		}
	}

	return 0;
}

