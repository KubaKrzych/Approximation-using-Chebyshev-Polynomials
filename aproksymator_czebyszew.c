#include "makespl.h"
#include "piv_ge_solver.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

// I - [-1, 1]
// w(x) = 1/(1-x^2)^(0.5) waga
// Tn(x) = cos(n * arccos(x)), as n = 0,1,2,..,oo 
// rekurencyjny zapis Tn(x) = 2x * Tn-1(x) - Tn-2(x) = 0.5 * ((x + (x^2 - 1)^0.5)^k + (x - (x^2 - 1)^0.5)^k), gdzie
// x = cos(t), k = 0,1,2,...

/*
 * Funkcje bazowe: 
 		n - liczba funkcji 
		a,b - granice przedzialu aproksymacji 
		i - numer funkcji 
		x - wspolrzedna dla ktorej obliczana jest wartosc funkcji
 */
double fi(int x, int stopien){
	if (stopien == 3)
		return 4*x^3 - 3*x;
	if (stopien == 2)
		return 2*x^2 - 1;
	if (stopien == 1){}
}

/* Pierwsza pochodna fi */
double dfi(int ilosc_funkcji, int stopien){
	return 
}

/* Druga pochodna fi */
double d2fi(int ilosc_funkcji, int stopien){}

/* Trzecia pochodna fi */
double d3fi(int ilosc_funkcji, int stopien){}

/* Pomocnicza f. do rysowania bazy */
double xfi(int ilosc_funkcji, int stopien, FILE *out){

}
