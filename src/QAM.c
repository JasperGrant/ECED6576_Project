// QAM.h
// Jasper Grant
// Script for extra functions definitions needed take a Gold Sequence, apply QAM and then transmit

#include "QAM.h"

// QAM
complex_signal QAM(const int_signal input) {
    // Init output
    complex_signal output = init_complex_signal(input.size / 2);

    int byte;
    for (int i = 0; i < input.size / 2; i++) {
        byte = input.data[2 * i] + 2 * input.data[2 * i + 1];
        switch (byte) {
            case 0:
                output.data[i] = -1 + I;
                break;
            case 1:
                output.data[i] = -1 - I;
                break;
            case 2:
                output.data[i] = +1 + I;
                break;
            case 3:
                output.data[i] = +1 - I;
                break;
        }
    }

    return output;
}

// Demodulate QAM
int_signal demod_QAM(const complex_signal input) {
    // Init output
    int_signal output = init_int_signal(input.size * 2);

    for (int i = 0; i < input.size; i++) {
        if (creal(input.data[i]) < 0) {
            output.data[2 * i] = 0;
        } else {
            output.data[2 * i] = 1;
        }

        if (cimag(input.data[i]) < 0) {
            output.data[2 * i + 1] = 0;
        } else {
            output.data[2 * i + 1] = 1;
        }
    }

    return output;
}
