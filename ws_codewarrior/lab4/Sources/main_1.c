/*
 * main implementation: use this 'C' sample to create your own application
 *
 */





#include "derivative.h" /* include peripheral declarations */
#include <stdio.h>
#include <string.h>


void fun1(){
	int internal=0;
	int internal2=0;
	int internal3=0;
	}

/* Se llama directamente a la funci�n y segun entra directamente se guarda el PC en la pila.
 * Desplazamos la pila n posiciones de memoria y cargamos en el resgistro la nueva direcci�n
 * del stack pointer pero sumandole el numero de argumentos de entrada, en este caso como no hay
 * argumento simplemente se a�ade 0.
 * El N de la linea anterior indica la reserva de memoria necesaria para la funci�n mas la propia 
 * direcci�n de la funci�n mas un word de retorno. (4+4+4)+4+4= Variables internas + return + funci�n. 
 * Se ejecuta el codigo interno.
 * Desdesplazamos el Stack pointer y se carga de nuevo y se saca el valor de la pila.
 * Por ultimo se desenlaza.
 * */
int fun2(){
	int internal = 0;
	internal++;
	return internal;
	}
/* Lo mismo que antes pero la variable se guarda y se carga en un registro para sacar su valor fuera 
 * de la funci�n.
 * */
int fun3(int a){
	a++;
	return a;
	}
// Carga en un registro el dato de entrada y llama a la funci�n.
/* Una vez dentro de la funci�n el aprametro de entrada que esta en un registro se vuelca a la pila
 * La salida es como en el anterior.
 * */
void fun4(int a, int* b){
	int internal = a;
	}
 /* Se pasan igual que en el anterior pero para pasar el valor por referencia se hace un 
  * direccionamiento cruzado.
  * */


int main(void)
{
	int a=1;
	int b;
	
	fun1();
	
//	 Llama directamente a la referencia de la funci�n.

	fun2();
	b = fun3(a);
	fun4(a, &b);
	
//	for(;;) {	   
//	   	counter++;
//	}
	
	return 0;
}
