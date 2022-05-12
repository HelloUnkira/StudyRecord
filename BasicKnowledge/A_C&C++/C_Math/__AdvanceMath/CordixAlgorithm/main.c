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
        printf("\nCordix:\n");
        printf("CordixSin(%lf) = %lf\n", t, Sin);
        printf("CordixCos(%lf) = %lf\n", t, Cos);
        printf("CordixTan(%lf) = %lf\n", t, Tan);
        printf("CordixArctan(x) = %lf\n", ATan);
        printf("\nC std lib:\n");
        printf("sin(%lf) = %lf\n", t, sin(t));
        printf("cos(%lf) = %lf\n", t, cos(t));
        printf("tan(%lf) = %lf\n", t, tan(t));
        printf("atan(x) = %lf\n",  atan2(sin(t), cos(t)));
    }
}
