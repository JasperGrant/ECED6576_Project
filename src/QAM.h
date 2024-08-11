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

real_signal mod_QAM(int_signal input, int carrier_freq, int sample_rate);

int_signal demod_QAM(real_signal input, int carrier_freq, int sample_rate);

#endif
