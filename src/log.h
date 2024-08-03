// Functions declarations to log int_signals, complex_signals and real_signals to files to later be plotted in Python
// Jasper Grant


#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#include "DSP.h"

// Function to log an int_signal to a file
void log_int_signal(int_signal signal, char *filename);

// Function to log a complex_signal to a file
void log_complex_signal(complex_signal signal, char *filename);

// Function to log a real_signal to a file
void log_real_signal(real_signal signal, char *filename);

#endif //LOG_H
