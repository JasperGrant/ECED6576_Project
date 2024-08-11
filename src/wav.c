// wav.c
// Jasper Grant
// Functions to convert back and forth between wav files and existing signal structures

#include "wav.h"

int write_wav_complex(complex_signal signal, char *filename, int sample_rate) {
    // Setup buffer for signal length
    short int buffer[signal.size];

    // Setup new wav header
    struct wav_header header;

    // Hard code in necessary strings
    strncpy(header.riff_tag, "RIFF", 4);
    strncpy(header.wave_tag, "WAVE", 4);
    strncpy(header.fmt_tag, "fmt ", 4);
    strncpy(header.data_tag, "data", 4);

    // Assign appropriate values for our signal
    header.chunk_size = 16;
    header.format = 1; // 1 for PCM
    header.num_channels = 1;
    header.sample_rate = 96000;
    header.bits_per_sample = 16;
    header.bytes_per_second = header.sample_rate * header.num_channels * header.bits_per_sample / 8;
    header.bytes_per_sample = header.num_channels * header.bits_per_sample / 8;

    // Put signal into buffer


    return 0;
}

complex_signal read_wav_complex(char *filename);
