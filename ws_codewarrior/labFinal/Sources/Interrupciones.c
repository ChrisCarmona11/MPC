

#include <constantes_int_1.h>

#define LDVAL_TRIGGER_1MS 20640 // 1ms/48.45ns = 20639.83

int buffer[128][4];
//Banderas para avisar al main
extern int PIT0_INTERRUP_FLAG;
extern int LPTMR_INTERRUPT_FLAG;

//COntadores
extern int contadorSegundos;
extern int contadorMilisegundos;

int canal;

#include <MK40DZ10.h>
void enable_irq (int irq)
{
    int div;
    
    /* Make sure that the IRQ is an allowable number. Right now up to 91 is 
     * used.
     */
    if (irq > 91)
        printf("\nERR! Invalid IRQ value passed to enable irq function!\n");
    
    /* Determine which of the NVICISERs corresponds to the irq */
    div = irq/32;
    
    switch (div)
    {
    	case 0x0:
              NVICICPR0 |= 1 << (irq%32);
              NVICISER0 |= 1 << (irq%32);
              break;
    	case 0x1:
              NVICICPR1 |= 1 << (irq%32);
              NVICISER1 |= 1 << (irq%32);
              break;
    	case 0x2:
              NVICICPR2 |= 1 << (irq%32);
              NVICISER2 |= 1 << (irq%32);
              break;
    }              
}

void lptmr_clear_registers() 
{
	LPTMR0_CSR = 0x00;
	LPTMR0_PSR = 0x00;
	LPTMR0_CMR = 0x00;
}

void pit_interrupt_config(void)
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
	enable_irq(68); // regist 2 y 4 de sobra asi que el bit flag será el 4 creo.
	// Enable timer and interrupt
	PIT_TCTRL0 = (PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK);
}


void lptmr_interrupt_config() {
	int compare_value = 999;  //value must be less than 0xFFFF

	//Enable Port clocks
	SIM_SCGC5 |=
			SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK
					| SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

	/* Enable LPT Module */SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;

	LPTMR_INTERRUPT_FLAG= 0; //Clear global variable

	//Reset LPTMR module
	lptmr_clear_registers();

	/* Enable LPT Interrupt in NVIC*/
	enable_irq(85); //LPTMR Vector is 101. IRQ# is 101-16=85

	/* Configure LPT */LPTMR0_CMR = LPTMR_CMR_COMPARE(compare_value); //Set compare value
	LPTMR0_PSR = LPTMR_PSR_PCS(0x1) | LPTMR_PSR_PBYP_MASK; //Use LPO clock and bypass prescale

	// Habilita la interrupcion del timer.
	LPTMR0_CSR = LPTMR_CSR_TIE_MASK;  //Enable LPT interrupt

	LPTMR0_CSR |= LPTMR_CSR_TEN_MASK; //Turn on LPTMR and start counting
	
	LPTMR_INTERRUPT_FLAG=0;

}

void lptmr_isr(void)
{
  LPTMR0_CSR|=LPTMR_CSR_TCF_MASK;  //Clear LPT Compare flag
  LPTMR_INTERRUPT_FLAG=1;  //Set global variable
  contadorSegundos++;
}

void pit0_isr(void) 
{
	PIT_TFLG0 |= PIT_TFLG_TIF_MASK; //Clear LPT Compare flag
	for (canal = 0; canal < 4; canal++) 
	{
		buffer[contadorMilisegundos][canal]= tabla[contadorMilisegundos][canal];
	}
	contadorMilisegundos++; //Aumentar el contador de milisegundos
	if (contadorMilisegundos == 128)
	{
		PIT0_INTERRUP_FLAG = 1;
		contadorMilisegundos = 0;
	}
}
