
#include <math.h>

#include <stdio.h>
#include <stdlib.h>

#define MATH_LIBRARY_H_SIMPLE_FUNCTION_TEST
#define MATH_LIBRARY_H_DEFINITE_INTEGRATION
#define MATH_LIBRARY_H_NETTON_TANGENT
#define MATH_LIBRARY_H_FFT_AND_IFFT_TEST
#include "math_library.h"

int main(int argc, char* argv[]) {
    
    test_simple_function();
    //test_definite_integration();
    //test_newton_tangent();
    //test_fft_and_ifft();
    return 0;
}

