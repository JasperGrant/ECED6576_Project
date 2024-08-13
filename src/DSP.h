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
#include <assert.h>

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

int_signal repeat_signal(int_signal signal, int repetitions);

// Generates Gold Code
int_signal generate_gold_code(int regCount, int shiftsize, int sampleCount, int rootSize);

complex_signal convolve(complex_signal x, real_signal h);

complex_signal upsample(complex_signal input, real_signal FIR, int up_factor, int offset);

// Downsamples the signal
complex_signal downsample(complex_signal input, int downfactor);

int_signal normalize(real_signal input);

int_signal get_signal_slice(int_signal whole_signal, int start, int end);

#endif
