#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "AdvanceMath.h"

static double f_x_t(double X)
{
    return (1 / (1 + X * X));
}



static double F_0_X(double X)
{
    return X * X * X - 2 * X * X - 4 * X - 7;
}

static double F_1_X(double X)
{
    return 3 * X * X - 4 * X - 4;
}



int main(int argc, char* argv[]) {
    
    printf("\n%lf",Cal_DI(f_x_t, 0.0, 1.0, 10, 1));
    printf("\n%lf",Cal_DI(f_x_t, 0.0, 1.0, 10, 2));
    
    printf("\n%lf",Cal_NT(F_0_X, F_1_X, 4, 0.0001));
    
    return 0;
}

