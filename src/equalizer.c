// equalizer.c
// Jasper Grant
// Functions to equalize a signal

#include "equalizer.h"

AdaptiveEqualizer *init_equalizer(int filter_order, double step_size, real_signal ideal_signal) {
    AdaptiveEqualizer *eq = (AdaptiveEqualizer *) malloc(sizeof(AdaptiveEqualizer));
    eq->filter_order = filter_order;
    eq->coefficients = (double *) calloc(filter_order, sizeof(double));
    eq->buffer = (double *) calloc(filter_order, sizeof(double));
    eq->step_size = step_size;
    eq->ideal_signal = ideal_signal;
    return eq;
}

void train_equalizer(AdaptiveEqualizer *equalizer, real_signal signal) {
    //Set up output signal
    real_signal output = init_real_signal(signal.size);

    // Main loop to go through each sample
    for (int i = 0; i < signal.size; i++) {
        // Shift buffer and feed in new data
        memmove(&equalizer->buffer[1], &equalizer->buffer[0], (equalizer->filter_order - 1) * sizeof(double));
        equalizer->buffer[0] = signal.data[i];

        // Calculate output of filter
        double y = 0.0;
        for (int j = 0; j < equalizer->filter_order; j++) {
            y += equalizer->coefficients[j] * equalizer->buffer[j];
        }

        // Calculate error
        double error = equalizer->ideal_signal.data[i] - y;

        // Training step
        for (int j = 0; j < equalizer->filter_order; j++) {
            equalizer->coefficients[j] += equalizer->step_size * error * equalizer->buffer[j];
        }
    }
}

real_signal use_equalizer(AdaptiveEqualizer *equalizer, real_signal signal) {
    real_signal output = init_real_signal(signal.size);

    for (int i = 0; i < signal.size; i++) {
        // Shift buffer and feed in new data
        memmove(&equalizer->buffer[1], &equalizer->buffer[0], (equalizer->filter_order - 1) * sizeof(double));
        equalizer->buffer[0] = signal.data[i];

        // Calculate output of filter
        double y = 0.0;
        for (int j = 0; j < equalizer->filter_order; j++) {
            y += equalizer->coefficients[j] * equalizer->buffer[j];
        }

        output.data[i] = y;
    }

    return output;
}

void free_equalizer(AdaptiveEqualizer *equalizer) {
    free(equalizer->coefficients);
    free(equalizer->buffer);
    free(equalizer);
}

