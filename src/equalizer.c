// equalizer.c
// Jasper Grant
// Functions to equalize a signal

#include "equalizer.h"

AdaptiveEqualizer *init_equalizer(int signal_size, int_signal ideal_signal) {
    AdaptiveEqualizer *eq = malloc(sizeof(AdaptiveEqualizer));
    eq->weights = (double *) malloc(signal_size * sizeof(double));
    eq->ideal_signal = ideal_signal;
    return eq;
}

void train_equalizer(AdaptiveEqualizer *equalizer, int_signal signal) {
    // Update weights
    for (int i = 0; i < signal.size; i++) {
        double error = equalizer->ideal_signal.data[i] - signal.data[i];
        equalizer->weights[i] += 0.1 * error * signal.data[i]; // Update weights with a step size of 0.01
    }
}

int_signal use_equalizer(AdaptiveEqualizer *equalizer, int_signal signal) {
    // Init output
    int_signal output = init_int_signal(signal.size);
    // Apply each weight
    for (int i = 0; i < signal.size; i++) {
        output.data[i] = (short) (signal.data[i] * equalizer->weights[i]);
    }

    return output;
}

