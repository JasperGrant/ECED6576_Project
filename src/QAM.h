// QAM.h
// Jasper Grant
// Script for extra functions declarations needed take a Gold Sequence, apply QAM and then transmit


// Header Guard
#ifndef QAM_H
#define QAM_H


#define _USE_MATH_DEFINES

#include <stdio.h>
#include "DSP.h"

// QAM
complex_signal QAM(int_signal input);

// QAM_TX
complex_signal QAM_TX(int M, int Nfft, int NbrSymbols, int CP_L);


#endif
