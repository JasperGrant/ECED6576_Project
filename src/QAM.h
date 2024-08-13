// QAM.h
// Jasper Grant
// Script for extra functions declarations needed take a Gold Sequence, apply QAM and then transmit


// Header Guard
#ifndef QAM_H
#define QAM_H


#define _USE_MATH_DEFINES

#include <stdio.h>
#include <assert.h>
#include "DSP.h"

complex_signal mod_QAM(int_signal input);

real_signal QAM_to_real_waveform(complex_signal input, int carrier_freq, int sample_rate);

complex_signal real_waveform_to_QAM(int_signal input, const int carrier_freq, const int sample_rate);

int_signal demod_QAM(complex_signal input);

#endif
