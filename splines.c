#include "splines.h"

#include <stdlib.h>

#define MALLOC_FAILED( P, SIZE ) (((P)=malloc( (SIZE)*sizeof( *(P))))==NULL)

/* Allocate memory for spx->{x,f,f1,f2,f3} and check whether memory allocation was succesfull */
int alloc_spl (spline_t * spl, int n)
{
  spl->n = n;
  spl->x = malloc(sizeof(*spl->x) * spl->n);
  spl->f = malloc(sizeof(*spl->f) * spl->n);
  spl->f1 = malloc(sizeof(*spl->f1) * spl->n);
  spl->f2 = malloc(sizeof(*spl->f2) * spl->n);
  spl->f3 = malloc(sizeof(*spl->f3) * spl->n);
  return spl->x == NULL || spl->f == NULL || spl->f1 == NULL || spl->f2 == NULL || spl->f3 == NULL;
}

int read_spl (FILE * inf, spline_t * spl)
{
  int i;
  if (fscanf (inf, "%d", &(spl->n)) != 1 || spl->n < 0)
    return 1;

  if (alloc_spl (spl, spl->n))
    return 1;

  for (i = 0; i < spl->n; i++)
    if (fscanf
        (inf, "%lf %lf %lf %lf %lf", spl->x + i, spl->f + i, spl->f1 + i,
         spl->f2 + i, spl->f3 + i) != 5)
      return 1;
  return 0;
}

void write_spl (spline_t * spl, FILE * ouf)
{
  int i;
  fprintf (ouf, "%d\n", spl->n);
  for (i = 0; i < spl->n; i++)
    fprintf (ouf, "%g %g %g %g %g\n", spl->x[i], spl->f[i], spl->f1[i],
             spl->f2[i], spl->f3[i]);
  fclose(ouf);
}

double value_spl (spline_t * spl, double x) // Taylor formula
{
  int i;
  double dx;

  for (i = spl->n - 1; i > 0; i--)
    if (spl->x[i] < x)
      break;

  dx = x - spl->x[i];

  return spl->f[i]
	+ dx * spl->f1[i]
	+ dx * dx / 2 *  spl->f2[i]
	+ dx * dx * dx / 6 * spl->f3[i];
}

/* Free memory allocated for spl */
void free_spline(spline_t *spl){
  free(spl->f1);
  free(spl->f2);
  free(spl->f3);
  free(spl->f);
  free(spl->x);
}
