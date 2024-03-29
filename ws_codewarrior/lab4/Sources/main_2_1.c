

#include "derivative.h" /* include peripheral declarations */
#include <stdio.h>
#include <string.h>

#define VALOR_OBJETIVO 999

void lptmr_time_counter_configure();
void lptmr_time_counter_run();
void lptmr_clear_registers();
/*
 * Delay function using the LPTMR module
 */
void lptmr_time_counter_configure()
/*Esta funcion configura los puertos de salida que se van a utilizar para el reloj de 1 KHz. El proceso para generar un interrupcion posee
 * los siguiente paso:
 * 1. Abrir los puertos donde se van a conectar el reloj
 * 2. Habilitar un reloj LPTMR
 * 3. Configurar el reloj
 * */

{
  int compare_value=VALOR_OBJETIVO;  //value must be less than 0xFFFF or 65535

	//Enable Port clocks
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;
	
	/* Enable LPT Module */
	SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;
	
	lptmr_clear_registers(); // Limpiar los registros del timer

	/* Configure LPTMR */
	LPTMR0_CMR=LPTMR_CMR_COMPARE(compare_value);  //Set compare value
	LPTMR0_PSR=LPTMR_PSR_PCS(0x1)|LPTMR_PSR_PBYP_MASK;  //Use LPO clock and bypass prescale

	// Habilitada el reloj 01 y hago or con PBYP para deshabilitarlo el modo bypass.

	LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //Turn on LPTMR with default settings
	//Para selccionar el bit de entrada 1 que es precisamente el reloj de 1KHz.
}

void lptmr_clear_registers()
/* Esta funcion simplemente limpia los diferences registros relacionados con el timer 0
 * */
{
  LPTMR0_CSR=0x00;
  LPTMR0_PSR=0x00;
  LPTMR0_CMR=0x00;
}

int main(void)
/* Gestionamos en el main la habilatación del Flag TCF con una operación bitwise. 
 * Imprimimos el valor del contador en ese momento y lo resetamos
 * */
{
	lptmr_time_counter_configure();
	  
	for(;;) {
		if((LPTMR0_CSR & LPTMR_CSR_TCF_MASK)!=0){
			
			printf("Current value of counter register CNR is %d\n",LPTMR0_CNR);
			LPTMR0_CSR|=LPTMR_CSR_TCF_MASK;
		}
	}
	return 0;
}
