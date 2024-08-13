// equalizer.h
// Jasper Grant
// Functions to equalize a signal

#ifndef EQUALIZER_H
#define EQUALIZER_H

#include "DSP.h"
#include "stdlib.h"

typedef struct {
    double *weights;
    int_signal ideal_signal;
} AdaptiveEqualizer;

AdaptiveEqualizer *init_equalizer(int signal_size, int_signal ideal_signal);

void train_equalizer(AdaptiveEqualizer *equalizer, int_signal signal);

int_signal use_equalizer(AdaptiveEqualizer *equalizer, int_signal signal);

#endif // EQUALIZER_H
