
#include "derivative.h" /* include peripheral declarations */

#include "DriversLCD/Driver_SLCD.h"
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <cmath>


#define CHARWIDTH 6
#define VALOR_OBJETIVO 999
#define ESCALA_INT_REAL  10.0 / 65535.0

//Banderas
int PIT0_INTERRUP_FLAG 		= 0;
int LPTMR_INTERRUPT_FLAG 	= 0;

//valores para contadores
int contadorSegundos = 0;
int contadorMilisegundos = 0;
static float scala = 10.0 /65535.0 ;

//regist donde guardar los contadores de ms
//int canalActual;			// Variable para ir atravesando el regist


int regist[128][4];
extern int buffer[128][4];

//Los numeros de la pantalla 

int unidades;
int decenas;
int centenas;

//Funciones del periferico PWM para el buzzer
extern void initPWM();
extern void _PWM(float DutyCycle);
extern void time_delay(unsigned int delay);
extern void Buzz(int numberOfBuzzez);

//Funcionesd e interrupciones 
extern void enable_irq(int irq); // Función para habilitar los regists de interrupción
extern void pit_interrupt_config(void);
extern void lptmr_interrupt_config();

extern void lptmr_clear_registers();
//Rutinas de interrupcion
extern void lptmr_isr(void);
extern void pit0_isr(void);

enum {
	
	MAXIMO_MINIMO 		= 0,
	PICO_PICO			= 1,
	MEDIA_ARITMETICA	= 2,
	MEDIA_QUADRATICA	= 3
	
} EstadosVisualizacion;


float toVolts(int number) 
{
	float valor = number * scala;
	return valor;
}

void De_Float_Cadena(float valor, int *array)
{
	int i=0;
    long Num_High;
	long Num_Low;

             //Separamos el float,
	Num_High = (int)(valor); //el valor por encima del punto se guarda en Num_High
	Num_Low = (int)(valor*1000)-Num_High*1000; // el valor por debajo del punto se guarda en Num_Low
	
	array[0]= Num_High/10;
	array[1]= Num_High%10;
	array[2]= (Num_Low/100);
	array[3]= ((Num_Low%100)/10);
	array[4]= (Num_Low%10);

}


void prettyPrint(int number) 
{
	unidades = number % 10;
	decenas = (number / 10) % 10;
	centenas = (number / 100) % 10;

	_SLCDModule_PrintNumber(unidades, CHARWIDTH * 3);
	_SLCDModule_PrintNumber( decenas, CHARWIDTH * 2);
	_SLCDModule_PrintNumber(centenas, CHARWIDTH * 1);
}

void printToSLCD (int *numberArray){
	int i = 0;
	int roomForMinus 	= 0;
	int roomForDott 	= 0;
	if (numberArray[3]<0)
	{
		_SLCDModule_PrintString("-",0);
		roomForMinus = 6;
	}
	for (i=0;i<5;i++)
	{
		int position = i*CHARWIDTH;
		if (i==2)
		{
			_SLCDModule_PrintString(".",roomForMinus+position);
			roomForDott=6;
		}
		_SLCDModule_PrintNumber(abs(numberArray[i]),position+roomForMinus+roomForDott);
	}
}

int main(void)
{

	//Configuracion de los perifericos
	pit_interrupt_config();
	lptmr_interrupt_config();
	_SLCDModule_Init();
	initPWM();

	int max[4] 			= { 0, 0, 0, 0 };
	int min[4] 			= { 0, 0, 0, 0 };
	int accumulative[4] = { 0, 0, 0, 0 };
	int pantalla[5] 	= { 0.0, 0.0, 0.0, 0.0 ,0.0 };;
	float quadratic[4] 	= { 0.0, 0.0, 0.0, 0.0 };
	float max_f[4]		= { 0.0, 0.0, 0.0, 0.0 };
	float min_f[4]		= { 0.0, 0.0, 0.0, 0.0 };
	float promedio[4]	= { 0.0, 0.0, 0.0, 0.0 };
	float promedioQ[4]	= { 0.0, 0.0, 0.0, 0.0 };
	
	int muestraActual 		= 0;
	int canalActual 		= 0;
	int datoMostradoActual 	= 0;
	
	
	while (1) 
	{
		if (PIT0_INTERRUP_FLAG == 1) 
		{
			memcpy(regist, buffer, sizeof(int) * 128 * 4); 	// volcar al registro la muesta entera
			memset(buffer, 0, sizeof(int) * 128 * 4); 			// Limpiar el buffer
			for (canalActual = 0; canalActual < 4; canalActual++) 
			{
				for (muestraActual = 0; muestraActual < 128; muestraActual++) 
				{
					if (regist[muestraActual][canalActual] > max[canalActual]) 
					{
						max[canalActual] = regist[muestraActual][canalActual];
					}
					if (regist[muestraActual][canalActual] < min[canalActual]) 
					{
						min[canalActual] = regist[muestraActual][canalActual];
					}
					accumulative[canalActual] += regist[muestraActual][canalActual];
					quadratic[canalActual] +=  toVolts(regist[muestraActual][canalActual]) * toVolts(regist[muestraActual][canalActual]);
					if ( muestraActual == 127) 
						PIT0_INTERRUP_FLAG = 0;
				}
			}
			
			memset(promedio, 0, sizeof(float)*4);
			memset(promedioQ, 0, sizeof(float)*4);
			

			for (canalActual = 0; canalActual < 4; canalActual++) 
			{
				max_f[canalActual] = toVolts(max[canalActual]);
				min_f[canalActual] = toVolts(min[canalActual]);
				promedio[canalActual] = toVolts((int) (((float) accumulative[canalActual]) / 128.0));
				promedioQ[canalActual] = sqrt(quadratic[canalActual] / 128.0);

				//Limpiar los registros de las variables que se visualizan.
				max[canalActual] = 0;
				min[canalActual] = 0;
				accumulative[canalActual] = 0;
				quadratic[canalActual] = 0;
			}
			
			memset(pantalla, 0, sizeof(int)*5);
			
			if (LPTMR_INTERRUPT_FLAG ==1)
			{
				LPTMR_INTERRUPT_FLAG =0;
				canalActual = (contadorSegundos%128)/30;
				
				//Esto se pone por que tarda mas de segundo en llegar aqui el main
				//por lo que ya ha cambiado el valor del contador de segundos y no se cumple el criterio de los
				if (contadorSegundos<2){
					_PWM(50);
					time_delay(1);
					_PWM(0);
					time_delay(1);
				}
				datoMostradoActual = (contadorSegundos%30)/5;
				if ((contadorSegundos%30)==0) Buzz(((contadorSegundos/30)%4));
				_SLCDModule_ClearLCD(NO_ARROWS);
				switch (datoMostradoActual)
				{
				    case MAXIMO_MINIMO:
				    	if ((contadorSegundos%5)<2)
				    	{
				    		De_Float_Cadena(max_f[canalActual],pantalla);
				    		printToSLCD(pantalla);
				    	}else{
				    		De_Float_Cadena(min_f[canalActual],pantalla);
				    		printToSLCD(pantalla);
				    	}
				    	break;
				    case PICO_PICO:
				    	De_Float_Cadena(max_f[canalActual]-min_f[canalActual],pantalla);
						printToSLCD(pantalla);
				    	break;
				    case MEDIA_ARITMETICA:
				    	De_Float_Cadena(promedio[canalActual],pantalla);
						printToSLCD(pantalla);
				    	break;
				    case MEDIA_QUADRATICA:
				    	De_Float_Cadena(promedioQ[canalActual],pantalla);
						printToSLCD(pantalla);
				    	break;
				    default:
				    	prettyPrint(contadorSegundos);			    	
				    	break;
				}
			}
		}
	}

	return 0;
}

