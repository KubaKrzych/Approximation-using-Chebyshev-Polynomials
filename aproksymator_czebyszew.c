#include "makespl.h"
#include "gaus/piv_ge_solver.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>


double fi(int i, double x){
	while(i >= 2)
		return 2*x*fi(i-1,x) - fi(i-2,x);
	if (i == 1)
		return x;
	else if (i == 0)
		return 1;
	else
		return 0;
}

/* First derivative of fi */
double dfi(int i, double x){
	while(i >= 2)
		return 2*fi(i-1,x) + 2*x*dfi(i-1, x) - dfi(i-2,x);
	if (i == 1)
		return 1;
	else if (i == 0)
		return 0;
}

/* Second derivative of fi */
double d2fi(int i, double x){
    while(i >= 3)
        return 2*x*d2fi(i-1, x) - d2fi(i-2, x) + 4*dfi(i-1, x);
	if (i == 2)
		return 4;
    else
        return 0;
}

/* Third derivative of fi */
double d3fi(int i, double x){
    while (i >= 4)
        return 2*x*d3fi(i-1,x) - d3fi(i-2,x) + 6*d2fi(i-1,x);
    if (i == 3)
        return 24;
    else
        return 0;
}

double xfi(int n, int i, FILE *out)
{
	fprintf( out, "# nb=%d, i=%d\n", n, i );
	fclose(out);
}

// based on original make_spl
void make_spl(points_t * pts, spline_t * spl) // creating spl file here
{

	matrix_t *eqs= NULL; // pointer on a structure
	double *x = pts->x; // values from points_t structure points_t
	double *y = pts->y;
	double a = x[0]; // the beginning of interval
	double b = x[pts->n - 1]; // the end of interval
	int i, j, k;
	// n is a number of pairs (x and f(x)) in points_t structure
	int nb = pts->n - 3 > 10 ? 10 : pts->n - 3; // if number of pairs is higher than 10, use 10
  char *nbEnv= getenv( "APPROX_BASE_SIZE" ); // envirionment variable

	if( nbEnv != NULL && atoi( nbEnv ) > 0 )
		nb = atoi( nbEnv );

	eqs = make_matrix(nb, nb + 1); // creating matrix (nb x nb+1 dimension) without values in it

#ifdef DEBUG
#define TESTBASE 500
	{
		FILE           *tst = fopen("debug_base_plot.txt", "w");
		double dx = (b - a) / (TESTBASE - 1);
		for( j= 0; j < nb; j++ )
			xfi( a, b, nb, j, tst );
		for (i = 0; i < TESTBASE; i++) {
			fprintf(tst, "%g", a + i * dx);
			for (j = 0; j < nb; j++) {
				fprintf(tst, " %g", fi  (j, a + i * dx));
				fprintf(tst, " %g", dfi (j, a + i * dx));
				fprintf(tst, " %g", d2fi(j, a + i * dx));
				fprintf(tst, " %g", d3fi(j, a + i * dx));
			}
			fprintf(tst, "\n");
		}
		fclose(tst);
	}
#endif

	for (j = 0; j < nb; j++) { // j - row
		for (i = 0; i < nb; i++) // i - column
			for (k = 0; k < pts->n; k++)// k - the number of pair from pts_t structure
			// summing up ratio of fiI(x)*fiJ(x) for A * X + B, for matrix A (in PDF file (4th page))
				add_to_entry_matrix(eqs, j, i, fi(i, x[k]) * fi(j, x[k]));

		// right side of the equation
		for (k = 0; k < pts->n; k++)
		// summing up ratio of fiI(x)*fiJ(x) for A * X + B, for matrix B ( y[k] = f(x(k)) )
			add_to_entry_matrix(eqs, j, nb, y[k] * fi(j, x[k]));
	}

#ifdef DEBUG
	write_matrix(eqs, stdout);
#endif

// solution of matrix from pdf Gaussian elimination, left matrix A goes to right side and we get:
// X = B * A^(-1)
	if (piv_ge_solver(eqs)) {
		spl->n = 0;
		return;
	}
#ifdef DEBUG
	write_matrix(eqs, stdout);
#endif

	if (alloc_spl(spl, nb) == 0) {
		for (i = 0; i < spl->n; i++) { // i - number of line
			double xx = spl->x[i] = a + i*(b-a)/(spl->n-1); // Shift x, from the beginning of the data range to the end and saving in spl-> x[i] that is the first column
			xx+= 10.0*DBL_EPSILON;  // protection against placing a point in the previous range
			spl->f[i] = 0;
			spl->f1[i] = 0;
			spl->f2[i] = 0;
			spl->f3[i] = 0;
			for (k = 0; k < nb; k++) {
				double ck = get_entry_matrix(eqs, k, nb); // extracting values ​​from the matrix, these are values ​​from matrix B
				spl->f[i] += ck * fi  (k, xx); // value * function
				spl->f1[i] += ck * dfi (k, xx); // value * derivative
				spl->f2[i] += ck * d2fi(k, xx); // value * 2diverative
				spl->f3[i] += ck * d3fi(k, xx); // value * 3diverative
			}
		}
	}
	// free matrix "eqs"
	free(eqs->e);	 
	free(eqs);

#ifdef DEBUG
	{
		FILE           *tst = fopen("debug_spline_plot.txt", "w");
		double		dx = (b - a) / (TESTBASE - 1);
		for (i = 0; i < TESTBASE; i++) {
			double yi= 0;
			double dyi= 0;
			double d2yi= 0;
			double d3yi= 0;
			double xi= a + i * dx;
			for( k= 0; k < nb; k++ ) {
							yi += get_entry_matrix(eqs, k, nb) * fi(k, xi);
							dyi += get_entry_matrix(eqs, k, nb) * dfi(k, xi);
							d2yi += get_entry_matrix(eqs, k, nb) * d2fi(k, xi);
							d3yi += get_entry_matrix(eqs, k, nb) * d3fi(k, xi);
			}
			fprintf(tst, "%g %g %g %g %g\n", xi, yi, dyi, d2yi, d3yi );
		}
		fclose(tst);
	}
#endif

}
