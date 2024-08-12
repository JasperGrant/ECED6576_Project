// equalizer.h
// Jasper Grant
// Functions to equalize a signal

#ifndef EQUALIZER_H
#define EQUALIZER_H

#include "DSP.h"
#include "stdlib.h"

typedef struct {
    int filter_order;
    double *coefficients;
    double *buffer;
    double step_size;
    real_signal ideal_signal;
} AdaptiveEqualizer;

AdaptiveEqualizer *init_equalizer(int filter_order, double step_size, real_signal ideal_signal);

void train_equalizer(AdaptiveEqualizer *equalizer, real_signal signal);

real_signal use_equalizer(AdaptiveEqualizer *equalizer, real_signal signal);

void free_equalizer(AdaptiveEqualizer *equalizer);

#endif // EQUALIZER_H
