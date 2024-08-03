// QAM.h
// Jasper Grant
// Script to take a Gold Sequence, apply QAM and then transmit
// Inclusions
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <complex.h>
#include <stdio.h>
#include "DSP.h"

// QAM
complex_signal QAM(int_signal input)
{
    complex_signal output;
    output.size = input.size/2;
    output.data = (complex double*) malloc(output.size*sizeof(complex double));

    int byte;
    for (int i = 0; i < input.size/2; i++)
    {
        byte = input.data[2*i] + 2*input.data[2*i+1];
        switch(byte)
        {
            case 0 :
                output.data[i] = -1+I;
                break;
            case 1 :
                output.data[i] = -1-I;
                break;
            case 2 :
                output.data[i] = +1+I;
                break;
            case 3 :
                output.data[i] = +1-I;
                break;

        }
    }
    
    return output;
}

complex_signal OFDM_TX(int M, int Nfft, int NbrSymbols,int CP_L)
{
    // For file handling
    FILE* file;
    char linebuffer[BUF];

    int k = log2(M);    // Bits per symbol
    int bits_per_symbol = 85;
    // int numBits = bits_per_symbol*NbrSymbols*k; // Number of bits to process

    int numSamplesPerSymbol = 24; // Oversampling factor

    // Gets input data from file
    int_signal data_in;
    data_in.size = 680;
    data_in.data = (short*) malloc(data_in.size*sizeof(short));

    #ifdef __WIN32
        file = fopen("txt\\OFDM_input.txt","r");
    #elif __unix__
        file = fopen("/mnt/mmcblk0p2/andrew/src/txt/OFDM_input.txt","r");
    #endif

    for(int i = 0; i < data_in.size; i++) 
    {
        fgets(linebuffer,BUF,file);
        data_in.data[i] = atof(linebuffer);
    }
    fclose(file);
    
    // Gets interleaver from file
    int_signal interl;
    interl.size = 170;
    interl.data = (short*) malloc(interl.size*sizeof(short));

    #ifdef __WIN32
        file = fopen("txt\\OFDM_interleaver.txt","r");
    #elif __unix__
        file = fopen("/mnt/mmcblk0p2/andrew/src/txt/OFDM_interleaver.txt","r");
    #endif

    for(int i = 0; i < interl.size; i++) 
    {
        fgets(linebuffer,BUF,file);
        interl.data[i] = atof(linebuffer);
    }
    fclose(file);

    int_signal input;
    input.size = interl.size;

    int_signal dataEncAll;
    dataEncAll.size = k*Nfft*(M-1) + interl.size*3;
    dataEncAll.data = (short*) malloc(dataEncAll.size*sizeof(short));

    // Generates Data Using Interleaver
    for (int m = 0; m < M; m++)
    {
        input.data = &data_in.data[m*interl.size];
        int_signal dataEnc = turboencode1(input,interl);

        for (int i = 0; i < dataEnc.size; i++)
        {
            dataEncAll.data[i+m*k*Nfft] = dataEnc.data[i];
        }
        free(dataEnc.data);
    }

    // 4-QA
    complex_signal dataMod = QAM4(dataEncAll);

    complex_signal input2;
    input2.size = Nfft;

    complex_signal stream;
    stream.size = (Nfft+CP_L)*M;
    stream.data = (complex double*) malloc(stream.size*sizeof(complex double));

    // Applies inverse fourier transform
    for (int m = 0; m < NbrSymbols; m++)
    {
        input2.data = &dataMod.data[m*Nfft];
        complex_signal ofdm = IDFT(input2);

        for (int i = 0; i < ofdm.size; i++)
        {
            ofdm.data[i] *= sqrt(Nfft);
        }
    
        memcpy(stream.data+m*(Nfft+CP_L),ofdm.data+Nfft-CP_L,CP_L*sizeof(complex double));
        memcpy(stream.data+m*(Nfft+CP_L)+CP_L,ofdm.data,Nfft*sizeof(complex double));

    }

    // Gets filter coefficients from file
    real_signal filter;
    filter.size = 241;
    filter.data = (double*) malloc(filter.size*sizeof(double));

    #ifdef __WIN32
        file = fopen("txt\\OFDM_filter.txt","r");
    #elif __unix__
        file = fopen("/mnt/mmcblk0p2/andrew/src/txt/OFDM_filter.txt","r");
    #endif

    for(int i = 0; i < filter.size; i++) 
    {
        fgets(linebuffer,BUF,file);
        filter.data[i] = atof(linebuffer);
    }
    fclose(file);


    return stream;

    // Upsamples and applies filter
    complex_signal txsignal = upsample(stream,filter,numSamplesPerSymbol,0);

    free(data_in.data);
    free(interl.data);
    free(dataEncAll.data);
    free(dataMod.data);
    //free(input2.data);
    free(stream.data);
    free(filter.data);

    return txsignal;
}