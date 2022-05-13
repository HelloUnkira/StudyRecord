#include <stdio.h>
#include <math.h>
#include "Cordix.h"

int main(int argc, char *argv[]) {
    
    for (int i = 0; i < 10; i++) {//测试arctan,sin,cos,tan
        double t = (double)i * 1.1;
        double Sin =  CordixSin(t);
        double Cos =  CordixCos(t);
        double Tan =  CordixTan(t);
        double ATan = CordixArctan(Sin, Cos);
        printf("\n\n");
        printf("CordixSin(%lf) = %.10lf, sin(%lf) = %.10lf\n", t, Sin, t, sin(t));
        printf("CordixCos(%lf) = %.10lf, cos(%lf) = %.10lf\n", t, Cos, t, cos(t));
        printf("CordixTan(%lf) = %.10lf, tan(%lf) = %.10lf\n", t, Tan, t, tan(t));
        printf("CordixArctan(x) = %.9lf, atan(x) = %.9lf\n", ATan, atan2(sin(t), cos(t)));
        printf("\n\n");
    }
}
