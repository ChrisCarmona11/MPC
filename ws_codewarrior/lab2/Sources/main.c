/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */

int main(void) {

	int a_int, b_int, c_int;
	unsigned int a_uint, b_uint, c_uint;
	float a_float, b_float, c_float;

	//Mirando el código en ensablandor 
	//podemos ver que ha reservado 36 espacios.
	//Dado que un word son 4 bites, 36/4 = 9, 
	//que es la cantidad de variables declarado.

	// Asignaciones 
	a_int = 1;  // escribir registro 
				// guardar en la pila 
	b_int = 2;
	c_int = 3;

	a_uint = 1;
	b_uint = 2;
	c_uint = 3;

	a_float = 1.0;
	b_float = 2.0;
	c_float = 3.0;

	//Operaciones + - * /

	//signed
	c_int = a_int + b_int;
	c_int = a_int - b_int;
	c_int = a_int * b_int;
	c_int = a_int / b_int;
	
	//Para valores enteros el target dispone
	//de funciones especificas que le permiten 
	//hacen las operaciones aritmeticas requeridas
	//adds subs mul.w (multiplicación datos de un solo word)
	//sdiv (signed division) 

	//unsigned
	c_uint = a_uint + b_uint;
	c_uint = a_uint - b_uint;
	c_uint = a_uint * b_uint;
	c_uint = a_uint / b_uint;
	
	//Para valores enteros el target dispone
	//de funciones especificas que le permiten 
	//hacen las operaciones aritmeticas requeridas
	//adds subs mul.w (multiplicación datos de un solo word)
	//udiv (unsigned division) 

	//float
	c_float = a_float + b_float;
	c_float = a_float - b_float;
	c_float = a_float * b_float;
	c_float = a_float / b_float;

	//En este caso el micro no dispone de instrucciones 
	//atomicas por lo que llama a una subrutina para 
	//calcular el resultado de la operación. En este 
	//el resultado se carga en r0 para luego volver a 
	//cargarse en la pila.  
	
	//Aritmeticas especiales
	
	a_int = b_int*16;
	a_int = b_int<<4;
	//Misma operacion de desplazamiento en ambos
	a_float = b_float*16;
	//a_float = b_float<<4; operación invalida
	a_int = b_int/16; 
	//se compara si b_int es cero antes que nada.
	// Y luego se hace un desplazamiento aritmetico a la derecha
	a_int = b_int>>4;
	// con desplazamento y ya
	a_float = b_float/16;
	//Mediante libreria
	//a_float = b_float>>4;
	
	// Conversión entre tipo de datos
	
	a_int = b_int*a_uint;
	a_uint = a_int*b_uint;
	//No hay conversión
	a_int = b_int*a_float;
	//usa 3 libs 2 de casteo y 1 para mul entre floats
	a_float = a_int*b_float;
	// 2 lib 1 casteo 1 de mul float
	a_float = a_uint*b_float;
	// 2 lib 1 casteo 1 de mul float
	
	
	for (;;) {
	}

	return 0;
}
