#include "makespl.h"
#include "piv_ge_solver.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

#define DEBUG

double fi(double x, int k){
	while(k >= 2)
		return 2*x*fi(x, k-1) - fi(x, k-2);
	if (k == 1)
		return x;
	else if (k == 0)
		return 1;
}

/* Pierwsza pochodna fi */
double dfi(double x, int k){
	while(k >= 2)
		return 2*fi(x, k-1) + 2*x*dfi(x,k-1) - dfi(x, k-2);
	if (k == 1)
		return 1;
	else if (k == 0)
		return 0;
}

/* Druga pochodna fi */
double d2fi(double x, int k){
    while(k >= 3)
        return 2*x*d2fi(x,k-1) - d2fi(x,k-2) + 4*dfi(x,k-1);
	if (k == 2)
		return 4;
    else 
        return 0;
}

/* Trzecia pochodna fi */
double d3fi(double x, int k){
    while ( k >= 4)
        return 2*x*d3fi(x, k-1) - d3fi(x, k-2) + 6*d2fi(x, k-1);
    if (k == 3)
        return 24;
    else 
        return 0;
}

double xfi(int n, int k, FILE *out)
{
	fprintf( out, "# nb=%d, i=%d\n", n, k );

}

void make_spl(points_t * pts, spline_t * spl)
{

	matrix_t       *eqs= NULL;
	double         *x = pts->x;
	double         *y = pts->y;
	double		a = x[0];
	double		b = x[pts->n - 1];
	int		i, j, k;
	int		nb = pts->n - 3 > 10 ? 10 : pts->n - 3;
  	char *nbEnv= getenv( "APPROX_BASE_SIZE" );

	if( nbEnv != NULL && atoi( nbEnv ) > 0 )
		nb = atoi( nbEnv );
	eqs = make_matrix(nb, nb + 1);

#ifdef DEBUG
#define TESTBASE 500
	{
		FILE           *tst = fopen("debug_base_plot.txt", "w");
		double		dx = (b - a) / (TESTBASE - 1);
		for( j= 0; j < nb; j++ )
			xfi(nb, j, tst);  //zmiana
		for (i = 0; i < TESTBASE; i++) {
			fprintf(tst, "%g",dx);
			for (j = 0; j < nb; j++) {
				fprintf(tst, " %g", fi  (j,dx));  //zmiana
				fprintf(tst, " %g", dfi (j,dx));  //zmiana
				fprintf(tst, " %g", d2fi(j,dx));  //zmiana
				fprintf(tst, " %g", d3fi(j,dx));  //zmiana
			}
			fprintf(tst, "\n");
		}
		fclose(tst);
	}
#endif

	for (j = 0; j < nb; j++) {
		for (i = 0; i < nb; i++)
			for (k = 0; k < pts->n; k++)
				add_to_entry_matrix(eqs, j, i, fi(i, x[k]) * fi(j, x[k]));

		for (k = 0; k < pts->n; k++)
			add_to_entry_matrix(eqs, j, nb, y[k] * fi(j, x[k]));
	}

#ifdef DEBUG
	write_matrix(eqs, stdout);
#endif

	if (piv_ge_solver(eqs)) {
		spl->n = 0;
		return;
	}
#ifdef DEBUG
	write_matrix(eqs, stdout);
#endif

	if (alloc_spl(spl, nb) == 0) {
		for (i = 0; i < spl->n; i++) {
			double xx = spl->x[i] = a + i*(b-a)/(spl->n-1);
			xx+= 10.0*DBL_EPSILON;  // zabezpieczenie przed ulokowaniem punktu w poprzednim przedziale
			spl->f[i] = 0;
			spl->f1[i] = 0;
			spl->f2[i] = 0;
			spl->f3[i] = 0;
			for (k = 0; k < nb; k++) {
				double		ck = get_entry_matrix(eqs, k, nb);
				spl->f[i]  += ck * fi  (k, xx);
				spl->f1[i] += ck * dfi (k, xx);
				spl->f2[i] += ck * d2fi(k, xx);
				spl->f3[i] += ck * d3fi(k, xx);
			}
		}
	}

#ifdef DEBUG
	{
		FILE           *tst = fopen("debug_spline_plot.txt", "w");
		double		dx = (b - a) / (TESTBASE - 1);
		for (i = 0; i < TESTBASE; i++) {
			double yi= 0;
			double dyi= 0;
			double d2yi= 0;
			double d3yi= 0;
			double xi=dx;
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
