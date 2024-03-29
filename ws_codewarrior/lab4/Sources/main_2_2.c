
#include "derivative.h" /* include peripheral declarations */
#include <stdio.h>
#include <string.h>

#define VALOR_OBJETIVO 999

#define LDVAL_TRIGGER_1MS 20639 // 1ms/48.45ns = 20639.83
// Esta constante ser� el n�mero de ciclos que tendr� que contar el reloj PIT  

int LPTMR_INTERRUPT, int_counter;

extern void enable_irq (int irq);

void lptmr_clear_registers()
{
  LPTMR0_CSR=0x00;
  LPTMR0_PSR=0x00;
  LPTMR0_CMR=0x00;
}
void lptmr_interrupt()
/* Esta funcion configura y habilita la interrupci�n. La diferencia es que en vez de comprobar un flag se
 * usa una variable global para detectar la interrupci�n (LPTMR_INTERRUPT)
 * */

{
  int compare_value=999;  //value must be less than 0xFFFF

  //Enable Port clocks
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

  /* Enable LPT Module */
  SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;

  LPTMR_INTERRUPT=0; //Clear global variable

  //Reset LPTMR module
  lptmr_clear_registers();

  /* Enable LPT Interrupt in NVIC*/
  enable_irq(85); //LPTMR Vector is 101. IRQ# is 101-16=85

  /* Configure LPT */
  LPTMR0_CMR=LPTMR_CMR_COMPARE(compare_value);  //Set compare value
  LPTMR0_PSR=LPTMR_PSR_PCS(0x1)|LPTMR_PSR_PBYP_MASK;  //Use LPO clock and bypass prescale
  
  // Habilita la interrupcion del timer.
  LPTMR0_CSR=LPTMR_CSR_TIE_MASK;  //Enable LPT interrupt

  LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //Turn on LPTMR and start counting

}

void lptmr_isr(void)
/* Rutina de servicio de antenci�on a la interrupci�n.
 * 
 * */
{
  LPTMR0_CSR|=LPTMR_CSR_TCF_MASK;  //Clear LPT Compare flag
  LPTMR_INTERRUPT=1;  //Set global variable
  int_counter++;
}



int main(void)
{
	lptmr_interrupt();
	  
	for(;;) {
		if(LPTMR_INTERRUPT == 1){
			
			LPTMR_INTERRUPT=0;
			printf("Time: %i \n", int_counter);
		}
	}
	
	return 0;
}
