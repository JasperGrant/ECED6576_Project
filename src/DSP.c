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

// Generates Gold Code
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

complex_signal convolve(complex_signal x, real_signal h) {
    // Variables
    int h_start, x_start, x_end;

    complex_signal y;
    y.size = h.size + x.size - 1;
    y.data = (complex double *) calloc(y.size, sizeof(complex double));

    // Convolution
    for (int i = 0; i < y.size; i++) {
        x_start = MAX(0, i-h.size+1);
        x_end = MIN(i+1, x.size);
        h_start = MIN(i, h.size-1);

        for (int j = x_start; j < x_end; j++) {
            y.data[i] += h.data[h_start--] * x.data[j];
        }
    }

    return y;
}

complex_signal upsample(complex_signal input, real_signal FIR, int up_factor, int offset) {
    complex_signal up;
    up.size = input.size * up_factor;
    up.data = (complex double *) malloc(up.size * sizeof(complex double));

    // Upsampling
    for (int i = 0; i < input.size; i++) {
        up.data[i * up_factor] = input.data[i];
    }

    // Applies FIR filter
    complex_signal y = convolve(up, FIR);


    complex_signal output;

    // The offset can be set to either 0 or 1 to match with
    // python and matlabs implementations of the upsampel function
    if (offset == 0) {
        output = y;
        output.size = (input.size - 1) * up_factor + FIR.size;
    }

    if (offset == 1) {
        output.size = up.size;
        output.data = y.data + (FIR.size / 2);
    }

    return output;
}


// Downsamples the signal
complex_signal downsample(complex_signal input, int downfactor) {
    // Creates output
    complex_signal output;
    output.size = input.size / (double) downfactor;
    output.data = (complex double *) malloc(output.size * sizeof(complex double));

    // Downsamples
    for (int i = 0; i < output.size; i++) {
        output.data[i] = input.data[i * downfactor];
    }

    return output;
}

// Normalizes a signal to a short int
int_signal normalize(real_signal input) {
    // Allocates memory for output signal
    int_signal output;
    output.size = input.size;
    output.data = (short *) malloc(output.size * sizeof(short));

    // Finds maximum of input signal
    double max = 0;
    for (int i = 0; i < input.size; i++) {
        if (fabs(input.data[i]) > max) {
            max = fabs(input.data[i]);
        }
    }

    // Writes output signal
    for (int i = 0; i < input.size; i++) {
        output.data[i] = (short) (input.data[i] * (pow(2, 15) - 1) / max);
    }

    return output;
}

int_signal get_signal_slice(int_signal whole_signal, int start, int end) {
    int_signal slice = init_int_signal(end - start);
    for (int i = start; i < end; i++) {
        slice.data[i - start] = whole_signal.data[i];
    }
    return slice;
}
