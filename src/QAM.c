// QAM.h
// Jasper Grant
// Script for extra functions definitions needed take a Gold Sequence, apply QAM and then transmit

#include "QAM.h"


// QAM
complex_signal mod_QAM(const int_signal input) {
    // Convert gold sequence to binary from BPSK
    // Loop through input
    for (int i = 0; i < input.size; i++) {
        // Convert to binary
        input.data[i] = input.data[i] == 1 ? 1 : 0;
    }
    // Init output
    complex_signal output = init_complex_signal(input.size / 2);

    for (int i = 0; i < input.size / 2; i++) {
        // QPSK
        // char to hold symbol from 0 to 3
        char binary_symbol = (input.data[2 * i + 1] & 0x01) + ((input.data[2 * i] & 0x01) << 1);
        // complex char to hold PSK symbol
        // Determine QPSK symbol from binary symbol
        switch (binary_symbol) {
            case 0b00: // 0
                output.data[i] = 1 + I;
                break;
            case 0b01: // 1
                output.data[i] = -1 + I;
                break;
            case 0b10: // 2
                output.data[i] = 1 - I;
                break;
            case 0b11: // 3
                output.data[i] = -1 - I;
                break;
            default:
                printf("Invalid binary symbol: %d\n", binary_symbol);
                assert(0);
        }
    }

    return output;
}

real_signal QAM_to_real_waveform(complex_signal input, const int carrier_freq, const int sample_rate) {
    real_signal output = init_real_signal(input.size);

    for (int i = 0; i < input.size; i++) {
        // Time
        double t = (double) i / sample_rate;
        // QPSK symbol
        complex double qpsk_symbol = input.data[i];
        // Real part
        output.data[i] = creal(qpsk_symbol) * cos(2 * M_PI * carrier_freq * t) - cimag(qpsk_symbol) * sin(
                             2 * M_PI * carrier_freq * t);
    }
    return output;
}

complex_signal real_waveform_to_QAM(real_signal input, const int carrier_freq, const int sample_rate) {
    complex_signal output = init_complex_signal(input.size);

    for (int i = 0; i < input.size; i++) {
        // Time
        double t = (double) i / sample_rate;
        // Real part
        double real = input.data[i] * cos(2 * M_PI * carrier_freq * t);
        // Imaginary part
        double imag = input.data[i] * sin(2 * M_PI * carrier_freq * t);
        // QPSK symbol
        output.data[i] = real + I * imag;
    }
    return output;
}


// Demodulate QAM
int_signal demod_QAM(complex_signal input) {
    int_signal output = init_int_signal(input.size * 2);

    // Loop through input
    for (int i = 0; i < input.size; i++) {
        double real = creal(input.data[i]);
        double imag = cimag(input.data[i]);
        // Determine binary symbol
        if (real > 0) {
            if (imag > 0) {
                output.data[2 * i] = -1;
                output.data[2 * i + 1] = -1;
            } else {
                output.data[2 * i] = -1;
                output.data[2 * i + 1] = 1;
            }
        } else {
            if (imag > 0) {
                output.data[2 * i] = 1;
                output.data[2 * i + 1] = -1;
            } else {
                output.data[2 * i] = 1;
                output.data[2 * i + 1] = 1;
            }
        }
    }

    return output;
}
