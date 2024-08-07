// DSP.h
// Andrew Doucet
// Modified by Jasper Grant
// Various Digital signal processing functions

# include "DSP.h"

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

// Performs a convolution
// Modified from code https://lloydrochester.com/post/c/convolution/
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

// Applies an IIR filter defined by a & b to x
// Assumes a0 =1 and then a and b are the same size
complex_signal IIR(real_signal b, real_signal a, complex_signal x) {
    // Creates output signal
    complex_signal y;
    y.size = x.size;
    y.data = (complex double *) malloc(y.size * sizeof(complex double));

    complex double B, A;

    // Loop for evrey element of x
    for (int i = 0; i < y.size; i++) {
        B = b.data[0] * x.data[i];
        A = 0;

        // Loop the for each filter element
        // Loops less if at the start to avoid negative indicies
        for (int j = 0; j < MIN(b.size-1, i); j++) {
            B += b.data[j + 1] * x.data[i - j - 1];
            A += a.data[j + 1] * y.data[i - j - 1];
        }

        y.data[i] = B - A;
    }

    return y;
}

// Usamples the input
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

// Resamples signal by upsampling and downsampling it
complex_signal resample(complex_signal input, real_signal filter, int upfactor, int downfactor) {
    return downsample(upsample(input, filter, upfactor, 0), downfactor);
}

// Puts the input signal on a carrier signal
real_signal mixing(complex_signal input, double fs, double fc) {
    real_signal output;
    output.size = input.size;
    output.data = (double *) malloc(output.size * sizeof(double));

    double t;
    for (int i = 0; i < output.size; i++) {
        t = i / fs;
        output.data[i] = creal(input.data[i] * cexp(-2 * I * M_PI * fc * t));
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

// Adds zeros to the end of the array
complex_signal zeros(complex_signal input, int Nzeros, int shift) {
    complex_signal output;
    output.size = input.size + Nzeros;
    output.data = (complex double *) calloc(output.size, sizeof(complex double));
    memcpy(output.data + shift, input.data, input.size * sizeof(complex double));
    return output;
}

// Adds zeros to the end of the array
int_signal int_zeros(int_signal input, int Nzeros, int shift) {
    int_signal output;
    output.size = input.size + Nzeros;
    output.data = (short *) calloc(output.size, sizeof(short));
    memcpy(output.data + shift, input.data, input.size * sizeof(short));
    return output;
}

// Returns the dft of a given signal
complex_signal DFT(complex_signal x) {
    // Creates signal
    complex_signal X;
    int N = X.size = x.size;
    X.data = (complex double *) calloc(x.size, sizeof(complex double));

    for (int n = 0; n < N; n++) {
        for (int k = 0; k < N; k++) {
            X.data[n] += x.data[k] * cexp(-I * 2 * M_PI * n * k / (double) N);
        }
    }

    return X;
}

// Returns the inverse dft of a given signal
complex_signal IDFT(complex_signal X) {
    // Creates signal
    complex_signal x;
    int N = x.size = X.size;
    x.data = (complex double *) calloc(x.size, sizeof(complex double));

    // Performs IFFT
    for (int n = 0; n < N; n++) {
        for (int k = 0; k < N; k++) {
            x.data[n] += (1 / (double) N) * X.data[k] * cexp(I * 2 * M_PI * n * k / (double) N);
        }
    }
    return x;
}

// FFT subfunction
void subFFT(complex double *x, complex double *X, int N, int s, int inv) {
    // Trivial case
    if (N == 1) {
        *X = *x;
    } else {
        subFFT(x, X, N / 2, s * 2, inv);
        subFFT(x + s, X + N / 2, N / 2, s * 2, inv);

        double complex p, q;

        for (int k = 0; k < N / 2; k++) {
            p = X[k];
            q = X[k + N / 2] * cexp(inv * -2 * I * M_PI * k / N);
            X[k] = p + q;
            X[k + N / 2] = p - q;
        }
    }
}

// Perfomrs a radix-2 FFT of an input signal
complex_signal FFT(complex_signal x) {
    complex_signal x_wz = zeros(x, 8192 - x.size, 0);

    complex_signal X;
    X.size = x_wz.size;
    X.data = (complex double *) malloc(X.size * sizeof(complex double));

    subFFT(x_wz.data, X.data, x.size, 1, 1);

    X.size = x.size;
    return X;
}

// Perfomrs a radix-2 IFFT of an input signal
complex_signal IFFT(complex_signal X) {
    complex_signal X_wz = zeros(X, 8192 - X.size, 0);

    complex_signal x;
    x.size = X_wz.size;
    x.data = (complex double *) malloc(x.size * sizeof(complex double));

    subFFT(X_wz.data, x.data, X.size, 1, -1);

    x.size = X.size;

    for (int i = 0; i < x.size; i++) {
        x.data[i] /= x.size;
    }

    return x;
}

// Performs a cross correlation
complex_signal xcorr(complex_signal x, complex_signal y) {
    int N = x.size + y.size - 1;

    // Makes x & y the size of M
    complex_signal x_wz = zeros(x, y.size - 1, 0);
    complex_signal y_wz = zeros(y, x.size - 1, 0);
    // FFT
    complex_signal X = FFT(x_wz);
    complex_signal Y = FFT(y_wz);

    // Multilpies X & conjugate of Y
    complex_signal Z;
    Z.size = N;
    Z.data = (double complex *) malloc(Z.size * sizeof(double complex));

    for (int i = 0; i < N; i++) {
        Z.data[i] = X.data[i] * conj(Y.data[i]);
    }

    // IFFT
    complex_signal z = IFFT(Z);

    return z;
}

// Performs multiple cross correlations of x on different portions of stream
complex_signal *xcorr_csv(complex_signal input, complex_signal ref, int xcorrs, char filename[]) {
    // Output pointer
    complex_signal *output = (complex_signal *) malloc(xcorrs * sizeof(complex_signal));
    complex_signal y;
    y.size = ref.size;

    int index;

    FILE *csv;
    csv = fopen(filename, "w");

    // Performing crosscorrelations and writes them to files
    for (int i = 0; i < xcorrs; i++) {
        index = (input.size - ref.size) * (i / ((double) xcorrs - 1));
        y.data = input.data + index;
        output[i] = xcorr(ref, y);

        // Writes to file
        for (int j = 0; j < output[i].size; j++) {
            fprintf(csv, "%lf,", cabs(output[i].data[j]));
        }
        fprintf(csv, "\n");
    }


    return output;
}

// Generates Gold Code
int_signal generate_gold_code(int regCount, int shiftSize, int sampleCount, int rootSize) {
    // Initializes output signal
    int_signal y = init_int_signal(sampleCount + 1);

    int N = pow(2, regCount) - 1;

    int len1, len2;
    int taps1[6], taps2[6];

    // Sets taps1 & taps2 depending on regCount
    switch (regCount) {
        case 3:
            len1 = len2 = 2;
            taps1[0] = taps2[0] = 3;
            taps1[1] = taps2[1] = 2;
            break;
        case 4:
            len1 = len2 = 2;
            taps1[0] = 4;
            taps1[1] = 1;
            taps2[0] = 4;
            taps2[1] = 3;
            break;
        case 5:
            len1 = 2;
            len2 = 4;
            taps1[0] = 5;
            taps1[1] = 2;
            taps2[0] = 5;
            taps2[1] = 4;
            taps2[2] = 3;
            taps2[3] = 2;
            break;
        case 6:
            len1 = 2;
            len2 = 4;
            taps1[0] = 6;
            taps1[1] = 1;
            taps2[0] = 6;
            taps2[1] = 5;
            taps2[2] = 2;
            taps2[3] = 1;
            break;
        case 7:
            len1 = 2;
            len2 = 4;
            taps1[0] = 7;
            taps1[1] = 3;
            taps2[0] = 7;
            taps2[1] = 3;
            taps2[2] = 2;
            taps2[3] = 1;
            break;
        case 9:
            len1 = 2;
            len2 = 4;
            taps1[0] = 9;
            taps1[1] = 4;
            taps2[0] = 9;
            taps2[1] = 6;
            taps2[2] = 4;
            taps2[3] = 3;
            break;
        case 10:
            len1 = 4;
            len2 = 6;
            taps1[0] = 10;
            taps1[1] = 8;
            taps1[2] = 5;
            taps1[3] = 1;
            taps2[0] = 10;
            taps2[1] = 7;
            taps2[2] = 6;
            taps2[3] = 4;
            taps2[4] = 2;
            taps2[5] = 1;
            break;
        case 11:
            len1 = 2;
            len2 = 4;
            taps1[0] = 11;
            taps1[1] = 2;
            taps2[0] = 11;
            taps2[1] = 8;
            taps2[2] = 5;
            taps2[3] = 2;
            break;
    }

    // Initializes reg1 & reg2
    int reg1[regCount], reg2[regCount];
    reg1[0] = reg2[0] = -1;
    for (int i = 1; i < regCount; i++) {
        reg1[i] = reg2[i] = 1;
    }

    // Intialization
    int prbs1[N], prbs2[N];
    int feedback1, feedback2;

    // Feedback through shift registers
    for (int k = 0; k < N; k++) {
        feedback1 = feedback2 = 1;

        for (int i = 0; i < len1; i++) {
            feedback1 *= reg1[taps1[i] - 1];
        }

        for (int i = 0; i < len2; i++) {
            feedback2 *= reg2[taps2[i] - 1];
        }

        prbs1[k] = reg1[regCount - 1];
        prbs2[k] = reg2[regCount - 1];

        // Updates reg1 & reg2
        for (int i = regCount - 1; i > 0; i--) {
            reg1[i] = reg1[i - 1];
            reg2[i] = reg2[i - 1];
        }

        reg1[0] = feedback1;
        reg2[0] = feedback2;
    }

    // Generates gold code
    int gold[N];
    for (int i = 0; i < N; i++) {
        gold[i] = -1 * prbs1[i] * prbs2[(i + shiftSize) % N];
    }

    // Adjusts for sample count and root size
    for (int i = 0; i < sampleCount; i++) {
        y.data[i] = (gold[i % rootSize] + 1) / 2;
    }

    return y;
}
