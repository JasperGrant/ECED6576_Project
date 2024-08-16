// equalizer.c
// Jasper Grant
// Functions to equalize a signal

#include "equalizer.h"

AdaptiveEqualizer *init_equalizer(int signal_size, int_signal ideal_signal)
{
    AdaptiveEqualizer *eq = malloc(sizeof(AdaptiveEqualizer));
    eq->weights = (double *)malloc(signal_size * sizeof(double));
    eq->ideal_signal = ideal_signal;

    // Init weights to 1
    for (int i = 0; i < signal_size; i++)
    {
        eq->weights[i] = 1.0;
    }
    return eq;
}

void train_equalizer(AdaptiveEqualizer *equalizer, int_signal signal)
{
    double mu = 0.1;

    // Update weights
    for (int i = 0; i < signal.size; i++)
    {
        // Calculate error
        double error = equalizer->ideal_signal.data[i] - signal.data[i];
        // Update weight
        equalizer->weights[i] += mu * error * signal.data[i];
    }
}

int_signal use_equalizer(AdaptiveEqualizer *equalizer, int_signal signal)
{
    // Init output
    int_signal output = init_int_signal(signal.size);
    // Apply each weight
    for (int i = 0; i < signal.size; i++)
    {
        output.data[i] = (short)(signal.data[i] * equalizer->weights[i]);
    }

    return output;
}
