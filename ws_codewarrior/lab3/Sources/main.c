/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */

void fun1();

int main(void) {
	int a = 3, b = 5, c = 8;

	if (a & b)
		fun1();
	
	//Hace un AND bitwise y lo guarda en r. Luego compara con 0(false). 
	//Mediante el registro de estado efectua el branch. si 0 a  lasiguiente 
	//instrucci�nm, si no ejecuta la fun1

	if (a | b)
		fun1();
	// Lo mismo que el anterior pero con la condici�n l�gica or.
	
	if (a && b)
		/* Aqu� en cambio, se comparan a y b con 0 individualmente,
		 * si a es distinto de 0 no hace la comparaci�n de b.
		 */
	
	if (a || b)
		fun1();

	//Lo mismo que el anterior pero con la diferencia de que si ya el 
	//primero es '0'(true) no comprueba b.
	
	//La ventaja de usar el operador doble es aprovechar para comparar primero 
	//la operaci�n logica menos compleja temporalmente, ahorrando asi tiempo de
	//ejecuci�n.  

	// 2. ejercicio

	if ((a || b || c) == 0)
		fun1();

	/* Como era de esperar, con el operador doble comprueba que cada
	 * uno de los operandos sea distinto de 0. Con que alguno de los
	 * uno de ellos no lo sea ejecuta fun1. En cambio, con el operador
	 * �nico mueve los operandos a registros y ejecuta la instruccion de
	 * or l�gico. El primero es m�s �ptimo.
	 */
	
	if ((a | b | c) == 0)
		fun1();
	/*Primero se hace el or bitwise de los primeros dos operandos y luego del resultado 
	 *del primero con el tercer operando. Se compara el resultado final con 0.
	 */
	if (!(a | b | c))
		fun1();
	//Este es completamente identico a la instrucci�n anterior para este micro.
	if ((!a) && (!b) && (!c)) {
		fun1();
		/*Compara cada operando con 0 hasta que encuentre el primer diferente a 0.
		 * Se salta al branch en cuanto el primer 1 aparece.
		 * */
	}
	if ((!a) & (!b) & (!c))
		fun1();
	/*
	 * El primero es muy eficiente, porque comprueba de forma independiente 
	 * si cada operando es igual a 0. Si uno de ellos no lo es, salta a la
	 * direcci�n de la siguiente estructura if. El segundo en cambio, va 
	 * haciendo comparaciones por parejas y va guardando en el registro r3 el
	 * resultado de cada comparaci�n, 1 o 0. Despu�s de hacer todas, al final, 
	 * compara r3 con 0, si es igual se sale y si no lo es se ejecuta fun1. Cabe
	 * remarcar que despu�s de cada comparaci�n, el valor que se guarda es un 1
	 * o un 0 de 8 bits, por lo que ejecta la instruccion uxtb despu�s de cada
	 * una para extenderlo a 32 bits. El m�s �ptimo de las dos opciones parece
	 * ser la primera estructura if.
	 */
	
	/*
	 * La funcion fun1 ser� llamada siempre que un operando sea diferente de 0.
	 * */ 

	//3. ejercicio

	if (a == 0) {
		fun();
	} else if (a == 1) {
		fun();
	} else if (a == 2) {
		fun();
	} else if (a == 3) {
		fun();
	} else if (a == 4) {
		fun();
	} else {
		fun();
	}
	
	/*
	 * Se compara cada una de las condiciones con su valor de uno en uno. En el peor de los casos
	 * pasaremos por todos los if antes de llegar al �ltimo else, lo que seri� una proceso muy lento.
	 * */

	switch (a) {
	case (0):
		fun1();
		break;
	case (1):
		fun1();
		break;
	case (2):
		fun1();
		break;
	case (3):
		fun1();
		break;
	case (4):
		fun1();
		break;
	default:
		fun1();
		break;
	}
	
	/*Se compara el valor a con el caso m�s alejado y se decide en funci�n de si el valor es menor o mayor
	 * a ese. S� es mayor, no estar�a identificado el caso, por lo que no har�a falta comprobar m�s
	 * pues los casos identificados son menores. Si es menor se carga en un registro la direcci�n del 
	 * programa donde est� la tabla de los valores case.
	 * 
	 * Posteriormente, se carga en el PC con direccionamiento indirecto el caso de la tabla de valores case
	 * anteriormente mencionada con el caso "exito"  y se ejecuta el codigo de ese case. Ver que como "a" es 
	 * 3, se llamar� a la linea 15b(15a tabulada). De aqui se ve que cualquier caso de la tabla case hubiera 
	 * tardado lo mismo. 
	 * */

	if (a == 0) {
		fun();
	} else if (a == 10) {
		fun();
	} else if (a == 50) {
		fun();
	} else if (a == 200) {
		fun();
	} else if (a == 500) {
		fun();
	} else {
		fun();
	}
	// Es lo mismo que con el anterior if. Solo cambia los numeros con los que se compara.

	switch (a) {
	case (0):
		fun1();
		break;
	case (10):
		fun1();
		break;
	case (50):
		fun1();
		break;
	case (200):
		fun1();
		break;
	case (500):
		fun1();
		break;
	default:
		fun1();
		break;
	}

/*En este segundo caso, la ventaja que ten�a la estructura case-switch frente al
 * if-elseif-else ya no est� presente. El if-elseif-else se procesa igual que en
 * el caso consecutivo, haciendo comparaciones de forma secuencial con los valores
 * inmediatos correspondientes. Es decir, se har� la misma cantidad de instrucciones
 * que se han hecho en el caso anterior. Por el contrario, la estructura case-switch
 * ha cambiado radicalmente, y se parece m�s a como se hace en con el if-elseif-else.
 * El compilador reconoce que no se trata de una comparaci�n consecutiva y por lo
 * tanto no forma una tabla de salto. Sin embargo, la estructra de c�digo generada
 * no id�ntica a la del if-elseif-else. Mientras que con if-elseif-else se ejecuta
 * fun1 justo despu�s de hacer la comparaci�n si esta se cumple, con case-switch se
 * hacen todas las comparaciones seguidas y en caso de que se cumpla alguna se salta
 * a la direcci�n donde est� fun1, en lugar de ejecutar lo haya en la instrucci�n 
 * siguiente. A nivel de optimizaci�n, case-sitch sigue siendo algo m�s �ptimo dado
 * que fun1 se almacena solo una vez en una direcci�n, aunque if-elseif-else es m�s
 * r�pido ya que si la condici�n se cumple fun1 es la instrucci�n siguiente a la
 * comparaci�n.
*/


// Ejercicio 4.

int i=0;
int j=0;
for (i; i<10 ;i++){
	fun1();
}

// Se carga en registro la variable de iteraci�n y se va incrementando por 1. Si se cumple la condici�n
//vuelve a cargar la variable de iteraci�n y asi hasta que no se cumpla la condici�n.
//En la primera oteraci�n ya se compueba la condici�n de iteraci�n.
while (j<10){
	fun1();
	j++;
}

/* Primero se salta a una branch donde esta el bloque de codigo del while que comprueba la condici�n.
 * Si se cumple vuelve del branch para ejecutar lo del interior del while y a continuaci�n vuelve a
 * por el bloque de c�digo que comprueba la condici�n. De esta manera saltar� hacia atras siempre que
 * cumpla la condici�n.
 * */


do {
	fun1();
	j++;
}while (j<10);


//Se ejecuta primero el bloque de codigo interno del while y se salta hacia atras si se cumple la condici�n.

/*Entre el for y el while la diferencia es minima pues internamente para el micro es muy parecido.
 *Cuando la condici�n para mantenerse dentro del ciclo no es un n�mero se recomienda usar un while, mientras
 *que cuando se usa un n�mero se suele usar un for. La diferencia de estos respecto al do while se da en la primera
 *iteraci�n pues esta se da siempre. En este caso la siguiente iteraci�n se daria si y solo si se cumple la
 *condici�n de iteraci�n.
 * */
	return 0;
}
