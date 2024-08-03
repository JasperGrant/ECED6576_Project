// DSP.h
// Andrew Doucet
// Modified by Jasper Grant
// Various Digital signal processing functions

// Header Guard
#ifndef DSP_H
#define DSP_H


// Inclusions
#define _USE_MATH_DEFINES

#include <complex.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

// Buffer size fore reading from files
#define BUF 99

// Returns largest of X & Y
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

// Returns smallest of X & Y
#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

// Contains a pointer to an array of short ints, and its size
typedef struct int_signal {
    int size;
    short *data;
} int_signal;

// Function to init data for int_signal
int_signal init_int_signal(int size);

// Contains a pointer to an array of doubles, and its size
typedef struct real_signal {
    int size;
    double *data;
} real_signal;

// Function to init data for real_signal
real_signal init_real_signal(int size);

// Contains a pointer to an array of complex doubles, and its size
typedef struct complex_signal {
    int size;
    double complex *data;
} complex_signal;

// Function to init data for complex_signal
complex_signal init_complex_signal(int size);

// Performs a convolution
complex_signal convolve(complex_signal x, real_signal h);

// Applies an IIR filter defined by a & b to x
// Assumes a0 =1 and then a and b are the same size
complex_signal IIR(real_signal b, real_signal a, complex_signal x);

// Upsamples the input
complex_signal upsample(complex_signal input, real_signal FIR, int up_factor, int offset);

// Downsamples the input
complex_signal downsample(complex_signal input, int downfactor);

// Resamples signal by upsampling and downsampling it
complex_signal resample(complex_signal input, real_signal filter, int upfactor, int downfactor);

// Puts the input signal on a carrier signal
real_signal mixing(complex_signal input, double fs, double fc);

// Normalizes a signal to a short int
int_signal normalize(real_signal input);

// Adds zeros to the end of the array
complex_signal zeros(complex_signal input, int Nzeros, int shift);

// Adds zeros to the end of the array
int_signal int_zeros(int_signal input, int Nzeros, int shift);

// Returns the dft of a given signal
complex_signal DFT(complex_signal x);

// Returns the inverse dft of a given signal
complex_signal IDFT(complex_signal X);

// FFT subfunction
void subFFT(complex double *x, complex double *X, int N, int s, int inv);

// Perfomrs a radix-2 FFT of an input signal
complex_signal FFT(complex_signal x);

// Perfomrs a radix-2 IFFT of an input signal
complex_signal IFFT(complex_signal X);

// Performs a cross correlation
complex_signal xcorr(complex_signal x, complex_signal y);

// Performs multiple cross correlations of x on different portions of stream
complex_signal *xcorr_csv(complex_signal input, complex_signal ref, int xcorrs, char filename[]);

// Generates Gold Code
int_signal generate_gold_code(int regCount, int shiftSize, int sampleCount, int rootSize);

#endif
