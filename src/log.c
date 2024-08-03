// Functions declarations to log int_signals, complex_signals and real_signals to files to later be plotted in Python
// Jasper Grant

#include "log.h"

// Function to log an int_signal to a file
void log_int_signal(int_signal signal, char *filename) {
    FILE *file = fopen(filename, "w");
    for (int i = 0; i < signal.size; i++) {
        fprintf(file, "%d\n", signal.data[i]);
    }
    fclose(file);
}

// Function to log a complex_signal to a file
void log_complex_signal(complex_signal signal, char *filename) {
    FILE *file = fopen(filename, "w");
    for (int i = 0; i < signal.size; i++) {
        fprintf(file, "%f,%f\n", creal(signal.data[i]), cimag(signal.data[i]));
    }
    fclose(file);
}

// Function to log a real_signal to a file
void log_real_signal(real_signal signal, char *filename) {
    FILE *file = fopen(filename, "w");
    for (int i = 0; i < signal.size; i++) {
        fprintf(file, "%f\n", signal.data[i]);
    }
    fclose(file);
}


