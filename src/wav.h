// wav.c
// Jasper Grant
// Functions to convert back and forth between wav files and existing signal structures

#ifndef WAV_H
#define WAV_H

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include "DSP.h"

struct wav_header {
    char riff_tag[4];
    int32_t flength;
    char wave_tag[4];
    char fmt_tag[4];
    int32_t chunk_size;
    int16_t format;
    int16_t num_channels;
    int32_t sample_rate;
    int32_t bytes_per_second;
    int16_t bytes_per_sample;
    int16_t bits_per_sample;
    char data_tag[4];
    int32_t dlength;
};

int write_wav_complex(complex_signal signal, char *filename, int sample_rate);

complex_signal read_wav_complex(char *filename);

#endif //WAV_H
