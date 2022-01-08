#include <stdio.h>
#include <stdlib.h>

double fi(double x, int k){
	while(k >= 2)
		return 2*x*fi(x, k-1) - fi(x, k-2);
	if (k == 1)
		return x;
	else if (k == 0)
		return 1;
}

double dfi(double x, int k){
	while(k >= 2)
		return 2*fi(x, k-1) + 2*x*dfi(x,k-1) - dfi(x, k-2);
	if (k == 1)
		return 1;
	else if (k == 0)
		return 0;
}
// 2dfi + 2x dfi(dfi)

double d2fi(double x, int k){
    while(k >= 3)
        return 2*x*d2fi(x,k-1) - d2fi(x,k-2) + 4*dfi(x,k-1);
        //return dfi(2*fi(x, k-2), k-1) + 2*x*d2fi(dfi(x,k-2),k - 1) + 2*dfi(fi(x, k-2), k-1) - d2fi(dfi(x, k-3),k-2);
	if (k == 2)
		return 4;
    else 
        return 0;
}
double d3fi(double x, int k){
    while ( k >= 4)
        return 2*x*d3fi(x, k-1) - d3fi(x, k-2) + 6*d2fi(x, k-1);
    if (k == 3)
        return 24;
    else 
        return 0;
}

int main(){
    printf("%lf\n", d3fi(2,7));
    return 0;
}