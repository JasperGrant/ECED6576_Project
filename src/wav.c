// wav.c
// Jasper Grant
// Functions to convert back and forth between wav files and existing signal structures

#include "wav.h"

int write_wav_real(int_signal signal, char *filename, int sample_rate) {
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
    header.sample_rate = sample_rate;
    header.bits_per_sample = 16;
    header.bytes_per_second = header.sample_rate * header.num_channels * header.bits_per_sample / 8;
    header.bytes_per_sample = header.num_channels * header.bits_per_sample / 8;

    header.dlength = signal.size * header.bytes_per_sample;
    header.flength = header.dlength + sizeof(struct wav_header);

    FILE *fp = fopen(filename, "wb");

    fwrite(&header, sizeof(struct wav_header), 1, fp);
    for (int i = 0; i < signal.size; i++) {
        fwrite(&signal.data[i], sizeof(short int), 1, fp);
    }

    fclose(fp);

    return 0;
}

int_signal read_wav_real(char *filename) {
    // Open file
    FILE *fp = fopen(filename, "rb");
    // Read header
    struct wav_header header;
    fread(&header, sizeof(struct wav_header), 1, fp);
    // Based on length of header read data
    int data_len = header.dlength / sizeof(short int);
    int_signal output = init_int_signal(data_len);
    short int temp = 0;
    for (int i = 0; i < data_len; i++) {
        fread(&temp, sizeof(short int), 1, fp);
        output.data[i] = (double) temp;
    }
    // Close file
    fclose(fp);

    return output;
}
