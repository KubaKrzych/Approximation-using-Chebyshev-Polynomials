#include "makespl.h"
#include "piv_ge_solver.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

//#define DEBUG

/* NOTE: The number of bases used can be set by the value of
          APPROX_BASE_SIZE environment variable
*/

/*
 * Base functions: n - number of functions a, b - limits of the approximation interval i
 * - number of the function x - coordinate for which the value of the function is calculated
 */


double fi(double a, double b, int n, int i, double x)
{
	double h = (b - a) / (n - 1);
	double h3 = h * h * h;
	int hi[5] = {i - 2, i - 1, i, i + 1, i + 2};
	double hx[5];
	int j;

	for (j = 0; j < 5; j++)
		hx[j] = a + h * hi[j];

	if ((x < hx[0]) || (x > hx[4]))
		return 0;
	else if (x >= hx[0] && x <= hx[1])
		return 1 / h3 * (x - hx[0]) * (x - hx[0]) * (x - hx[0]);
	else if (x > hx[1] && x <= hx[2])
		return 1 / h3 * (h3 + 3 * h * h * (x - hx[1]) + 3 * h * (x - hx[1]) * (x - hx[1]) - 3 * (x - hx[1]) * (x - hx[1]) * (x - hx[1]));
	else if (x > hx[2] && x <= hx[3])
		return 1 / h3 * (h3 + 3 * h * h * (hx[3] - x) + 3 * h * (hx[3] - x) * (hx[3] - x) - 3 * (hx[3] - x) * (hx[3] - x) * (hx[3] - x));
	else /* if (x > hx[3]) && (x <= hx[4]) */
		return 1 / h3 * (hx[4] - x) * (hx[4] - x) * (hx[4] - x);
}

/* First derivative of fi */
double dfi(double a, double b, int n, int i, double x)
{
	double		h = (b - a) / (n - 1);
	double		h3 = h * h * h;
	int		hi         [5] = {i - 2, i - 1, i, i + 1, i + 2};
	double		hx      [5];
	int		j;

	for (j = 0; j < 5; j++)
		hx[j] = a + h * hi[j];

	if ((x < hx[0]) || (x > hx[4]))
		return 0;
	else if (x >= hx[0] && x <= hx[1])
		return 3 / h3 * (x - hx[0]) * (x - hx[0]);
	else if (x > hx[1] && x <= hx[2])
		return 1 / h3 * (3 * h * h + 6 * h * (x - hx[1]) - 9 * (x - hx[1]) * (x - hx[1]));
	else if (x > hx[2] && x <= hx[3])
		return 1 / h3 * (-3 * h * h - 6 * h * (hx[3] - x) + 9 * (hx[3] - x) * (hx[3] - x));
	else /* if (x > hx[3]) && (x <= hx[4]) */
		return -3 / h3 * (hx[4] - x) * (hx[4] - x);
}

/* Second derivative of fi */
double d2fi(double a, double b, int n, int i, double x)
{
	double		h = (b - a) / (n - 1);
	double		h3 = h * h * h;
	int		hi         [5] = {i - 2, i - 1, i, i + 1, i + 2};
	double		hx      [5];
	int		j;

	for (j = 0; j < 5; j++)
		hx[j] = a + h * hi[j];

	if ((x < hx[0]) || (x > hx[4]))
		return 0;
	else if (x >= hx[0] && x <= hx[1])
		return 6 / h3 * (x - hx[0]);
	else if (x > hx[1] && x <= hx[2])
		return 1 / h3 * (6 * h - 18 * (x - hx[1]));
	else if (x > hx[2] && x <= hx[3])
		return 1 / h3 * (6 * h  -18 * (hx[3] - x));
	else /* if (x > hx[3]) && (x <= hx[4]) */
		return 6 / h3 * (hx[4] - x);
}

/* Third derivative of fi */
double d3fi(double a, double b, int n, int i, double x)
{
	double h = (b - a) / (n - 1);
	double h3 = h * h * h;
	int hi[5] = {i - 2, i - 1, i, i + 1, i + 2};
	double hx[5];
	int j;

	for (j = 0; j < 5; j++)
		hx[j] = a + h * hi[j];

	if ((x < hx[0]) || (x > hx[4]))
		return 0;
	else if (x >= hx[0] && x <= hx[1])
		return 6 / h3;
	else if (x > hx[1] && x <= hx[2])
		return -18 / h3;
	else if (x > hx[2] && x <= hx[3])
		return 18 / h3;
	else /* if (x > hx[3]) && (x <= hx[4]) */
		return -6 / h3;
}

/* Auxiliary function for drawing the base */
double xfi(double a, double b, int n, int i, FILE *out)
{
	double h = (b - a) / (n - 1);
	double h3 = h * h * h;
	int hi[5] = {i - 2, i - 1, i, i + 1, i + 2};
	double hx[5];
	int j;

	for (j = 0; j < 5; j++)
		hx[j] = a + h * hi[j];

	fprintf( out, "# nb=%d, i=%d: hi=[", n, i );
	for( j= 0; j < 5; j++ )
		fprintf( out, " %d", hi[j] );
	fprintf( out, "] hx=[" );
	for( j= 0; j < 5; j++ )
		fprintf( out, " %g", hx[j] );
	fprintf( out, "]\n" );
}

void make_spl(points_t * pts, spline_t * spl) // creating spl file here
{

	matrix_t *eqs= NULL; // pointer on a structure
	double *x = pts->x; // values from points_t structure points_t
	double *y = pts->y;
	double a = x[0]; // the beginning of interval
	double b = x[pts->n - 1]; // the end of interval
	int	i, j, k;
	// n is a number of pairs (x and f(x)) in points_t structure
	int	nb = pts->n - 3 > 10 ? 10 : pts->n - 3; // if number of pairs is higher than 10, use 10
  char *nbEnv= getenv( "APPROX_BASE_SIZE" ); // envirionment variable

	if( nbEnv != NULL && atoi( nbEnv ) > 0 )
		nb = atoi( nbEnv );

	eqs = make_matrix(nb, nb + 1); // creating matrix (nb x nb+1 dimension) without values in it

#ifdef DEBUG
#define TESTBASE 500
	{
		FILE *tst = fopen("debug_base_plot.txt", "w");
		double dx = (b - a) / (TESTBASE - 1);
		for( j= 0; j < nb; j++ )
			xfi( a, b, nb, j, tst );
		for (i = 0; i < TESTBASE; i++) {
			fprintf(tst, "%g", a + i * dx);
			for (j = 0; j < nb; j++) {
				fprintf(tst, " %g", fi  (a, b, nb, j, a + i * dx));
				fprintf(tst, " %g", dfi (a, b, nb, j, a + i * dx));
				fprintf(tst, " %g", d2fi(a, b, nb, j, a + i * dx));
				fprintf(tst, " %g", d3fi(a, b, nb, j, a + i * dx));
			}
			fprintf(tst, "\n");
		}
		fclose(tst);
	}
#endif


	for (j = 0; j < nb; j++) { // j - row
		for (i = 0; i < nb; i++) // i - column
			for (k = 0; k < pts->n; k++)	// k - the number of pair from pts_t structure
			// summing up ratio of fiI(x)*fiJ(x) for A * X + B, for matrix A (in PDF file (4th page))
				add_to_entry_matrix(eqs, j, i, fi(a, b, nb, i, x[k]) * fi(a, b, nb, j, x[k]));

		// right side of the equation
		for (k = 0; k < pts->n; k++)
		// summing up ratio of fiI(x)*fiJ(x) for A * X + B, for matrix B ( y[k] = f(x(k)) )
			add_to_entry_matrix(eqs, j, nb, y[k] * fi(a, b, nb, j, x[k]));
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
		for (i = 0; i < spl->n; i++) // i - number of line
		{
			double xx = spl->x[i] = a + i*(b-a)/(spl->n-1); // Shift x, from the beginning of the data range to the end and saving in spl-> x[i] that is the first column
			xx+= 10.0*DBL_EPSILON;  // protection against placing a point in the previous range
			spl->f[i] = 0;
			spl->f1[i] = 0;
			spl->f2[i] = 0;
			spl->f3[i] = 0;
			for (k = 0; k < nb; k++) {
				double ck = get_entry_matrix(eqs, k, nb); // extracting values ​​from the matrix, these are values ​​from matrix B
				spl->f[i]  += ck * fi  (a, b, nb, k, xx); // value * function
				spl->f1[i] += ck * dfi (a, b, nb, k, xx); // value * derivative
				spl->f2[i] += ck * d2fi(a, b, nb, k, xx); // value * 2diverative
				spl->f3[i] += ck * d3fi(a, b, nb, k, xx); // value * 3diverative
			}
		}
	}

#ifdef DEBUG
	{
		FILE *tst = fopen("debug_spline_plot.txt", "w");
		double dx = (b - a) / (TESTBASE - 1);
		for (i = 0; i < TESTBASE; i++) {
			double yi= 0;
			double dyi= 0;
			double d2yi= 0;
			double d3yi= 0;
			double xi= a + i * dx;
			for( k= 0; k < nb; k++ ) {
							yi += get_entry_matrix(eqs, k, nb) * fi(a, b, nb, k, xi);
							dyi += get_entry_matrix(eqs, k, nb) * dfi(a, b, nb, k, xi);
							d2yi += get_entry_matrix(eqs, k, nb) * d2fi(a, b, nb, k, xi);
							d3yi += get_entry_matrix(eqs, k, nb) * d3fi(a, b, nb, k, xi);
			}
			fprintf(tst, "%g %g %g %g %g\n", xi, yi, dyi, d2yi, d3yi );
		}
		fclose(tst);
	}
#endif

}
