#include "makespl.h"
#include "piv_ge_solver.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

// I - [-1, 1]
// G(x) = 1/(1-x^2)^(0.5)
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
double fi(double a, double b, int n, int i, double x)

/* Pierwsza pochodna fi */
double dfi(double a, double b, int n, int i, double x)

/* Druga pochodna fi */
double d2fi(double a, double b, int n, int i, double x)

/* Trzecia pochodna fi */
double d3fi(double a, double b, int n, int i, double x)

/* Pomocnicza f. do rysowania bazy */
double xfi(double a, double b, int n, int i, FILE *out)