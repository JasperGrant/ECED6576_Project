// DSP.h
// Andrew Doucet
// Modified by Jasper Grant
// Various Digital signal processing functions

#include "DSP.h"

// Initializes an int signal
int_signal init_int_signal(const int size) {
    int_signal signal;
    signal.size = size;
    signal.data = (short *) malloc(size * sizeof(short));
    return signal;
}

// Initializes a real signal
real_signal init_real_signal(const int size) {
    real_signal signal;
    signal.size = size;
    signal.data = (double *) malloc(size * sizeof(double));
    return signal;
}

// Initializes a complex signal
complex_signal init_complex_signal(const int size) {
    complex_signal signal;
    signal.size = size;
    signal.data = (complex double *) malloc(size * sizeof(complex double));
    return signal;
}

int_signal repeat_signal(int_signal signal, int repetitions) {
    int_signal repeated_signal = init_int_signal(signal.size * repetitions);

    // Repeat signal
    for (int i = 0; i < repetitions; i++) {
        for (int j = 0; j < signal.size; j++) {
            repeated_signal.data[i * signal.size + j] = signal.data[j];
        }
    }

    return repeated_signal;
}

//Generates Gold Code
int_signal generate_gold_code(int regcount, int shiftsize, int sampleCount, int rootSize) {
    int N = (int) pow(2, regcount) - 1;

    int *taps1, *taps2;
    int len1, len2;

    // Sets taps1 & taps2 depending on regcount
    switch (regcount) {
        case 3:
            len1 = len2 = 2;
            taps1 = (int[]){3, 1};
            taps2 = (int[]){3, 2};
            break;
        case 4:
            len1 = len2 = 2;
            taps1 = (int[]){4, 1};
            taps2 = (int[]){4, 3};
            break;
        case 5:
            len1 = 2;
            len2 = 4;
            taps1 = (int[]){5, 2};
            taps2 = (int[]){5, 4, 3, 2};
            break;
        case 6:
            len1 = 2;
            len2 = 4;
            taps1 = (int[]){6, 1};
            taps2 = (int[]){6, 5, 2, 1};
            break;
        case 7:
            len1 = 2;
            len2 = 4;
            taps1 = (int[]){7, 3};
            taps2 = (int[]){7, 3, 2, 1};
            break;
        case 9:
            len1 = 2;
            len2 = 4;
            taps1 = (int[]){9, 4};
            taps2 = (int[]){9, 6, 4, 3};
            break;
        case 10:
            len1 = 4;
            len2 = 6;
            taps1 = (int[]){10, 8, 5, 1};
            taps2 = (int[]){10, 7, 6, 4, 2, 1};
            break;
        case 11:
            len1 = 2;
            len2 = 4;
            taps1 = (int[]){11, 2};
            taps2 = (int[]){11, 8, 5, 2};
            break;
        default:
            // Handle unsupported regcount
            return init_int_signal(0);
    }

    // Initialization
    int *reg = (int *) malloc(regcount * sizeof(int));
    for (int i = 0; i < regcount; i++) {
        reg[i] = 1;
    }
    reg[0] = -1;

    int *prbs = (int *) malloc(N * sizeof(int));
    int *prbs_2 = (int *) malloc(N * sizeof(int));

    // Feedback through shift registers for prbs
    for (int k = 0; k < N; k++) {
        int fdbck = 1;
        for (int i = 0; i < len1; i++) {
            fdbck *= reg[taps1[i] - 1];
        }
        prbs[k] = reg[regcount - 1];
        for (int i = regcount - 1; i > 0; i--) {
            reg[i] = reg[i - 1];
        }
        reg[0] = fdbck;
    }

    // Reinitialize reg for prbs_2
    for (int i = 0; i < regcount; i++) {
        reg[i] = 1;
    }
    reg[0] = -1;

    // Feedback through shift registers for prbs_2
    for (int k = 0; k < N; k++) {
        int fdbck = 1;
        for (int i = 0; i < len2; i++) {
            fdbck *= reg[taps2[i] - 1];
        }
        prbs_2[k] = reg[regcount - 1];
        for (int i = regcount - 1; i > 0; i--) {
            reg[i] = reg[i - 1];
        }
        reg[0] = fdbck;
    }

    // Shift prbs_2
    int *prbs_2sh = (int *) malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) {
        prbs_2sh[i] = prbs_2[(i + shiftsize) % N];
    }

    // Generate gold code
    int *gold = (int *) malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) {
        gold[i] = -1 * prbs[i] * prbs_2sh[i];
    }

    // Adjust for rootSize
    int *goldRoot = (int *) malloc(rootSize * sizeof(int));
    for (int i = 0; i < rootSize; i++) {
        goldRoot[i] = gold[i % N];
    }

    // Repeat goldRoot to match sampleCount
    int_signal y = init_int_signal(sampleCount);
    for (int i = 0; i < sampleCount; i++) {
        y.data[i] = goldRoot[i % rootSize];
    }

    // Free allocated memory
    free(reg);
    free(prbs);
    free(prbs_2);
    free(prbs_2sh);
    free(gold);
    free(goldRoot);

    return y;
}

real_signal upsample(real_signal signal, int factor) {
    real_signal output = init_real_signal(signal.size * factor);

    for (int i = 0; i < signal.size; i++) {
        for (int j = 0; j < factor; j++) {
            output.data[i * factor + j] = signal.data[i];
        }
    }

    return output;
}
