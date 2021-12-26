#include <stdio.h>

// I - [-1, 1]
// G(x) = 1/(1-x^2)^(0.5)
// Tn(x) = cos(n * arccos(x)), as n = 0,1,2,..,oo
// rekurencyjny zapis Tn(x) = 2x * Tn-1(x) - Tn-2(x) = 0.5 * ((x + (x^2 - 1)^0.5)^k + (x - (x^2 - 1)^0.5)^k), gdzie
// x = cos(t), k = 0,1,2,...