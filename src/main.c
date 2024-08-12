// QAM.h
// Jasper Grant
// Script to take a Gold Sequence, apply QAM and then transmit

#include <iso646.h>

#include "QAM.h"
#include "log.h"
#include "wav.h"

#define CARRIER_FREQ 8000
#define SAMPLE_RATE 96000
#define SYMBOL_RATE 600

int main(void) {
    // Generate gold code
    int_signal gold = generate_gold_code(11, 0, 2048, 2047);

    // 2048 symbols
    // In QAM reduced to 1024 symbols

    // To get about 600 QAM symbols a second we need about 24 gold signals

    int_signal repeated_gold = repeat_signal(gold, 24);

    // Log gold code
    //log_int_signal(repeated_gold, "../../log/gold_code.csv");

    // Apply QAM and carrier freq
    complex_signal qam = mod_QAM(repeated_gold);

    // Log QAM
    log_complex_signal(qam, "../../log/qam.csv");

    // Get filter from file
    real_signal filter_1 = load_real_signal("../../filters/stage_1.csv", 41);

    // Upsample with pulse shaping filter
    complex_signal pulse_shaped = upsample(qam, filter_1, 4, 0);

    // Get second stage filter from file
    real_signal filter_2 = load_real_signal("../../filters/stage_2.csv", 801);

    // Upsample with second stage filter
    complex_signal upsampled = upsample(pulse_shaped, filter_2, 40, 0);

    // QAM to real waveform
    real_signal upconverted = QAM_to_real_waveform(upsampled, CARRIER_FREQ, SAMPLE_RATE);

    // Save QAM in wav file
    write_wav_real(upconverted, "../../wav/qam.wav", SAMPLE_RATE, 1);

    //real_signal qam_unwaved = read_wav_real("../../wav/qam.wav", 1);

    //log_real_signal(qam_unwaved, "../../log/qam_unwaved.csv");

    // Transmit QAM
    // Simulate channel with model


    // Apply QAM demodulation and downsample
    //int_signal demod = demod_QAM(qam, CARRIER_FREQ, SAMPLE_RATE);

    // Log demodulated signal
    //log_int_signal(demod, "../../log/demod.csv");

    // Equalize somewhere in here
}
