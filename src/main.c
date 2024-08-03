// QAM.h
// Jasper Grant
// Script to take a Gold Sequence, apply QAM and then transmit

#include <stdio.h>
#include <complex.h>
#include "QAM.h"

int main (void)
{
    printf("Hello, World!\n");

    complex_signal my_sig;
    real_signal my_sig_2;

    convolve(my_sig, my_sig_2);

    

    printf("Size of complex double: %lu\n", sizeof(complex double));
    return 0;
}