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
    log_int_signal(repeated_gold, "../../log/gold_code.csv"); // Log repeated gold code

    // Apply QAM
    complex_signal qam = mod_QAM(repeated_gold);
    log_complex_signal(qam, "../../log/qam.csv"); // Log QAM

    // Get filter from file
    real_signal filter_1 = load_real_signal("../../filters/stage_1.csv", 41);
    // Upsample with pulse shaping filter
    complex_signal pulse_shaped = upsample(qam, filter_1, 4, 0);
    log_complex_signal(pulse_shaped, "../../log/pulse_shaped.csv"); // Log pulse shaped signal

    // Get second stage filter from file
    real_signal filter_2 = load_real_signal("../../filters/stage_2.csv", 801);
    // Upsample with second stage filter
    complex_signal upsampled = upsample(pulse_shaped, filter_2, 40, 0);
    log_complex_signal(upsampled, "../../log/upsampled.csv"); // Log upsampled signal

    // QAM to real waveform
    real_signal upconverted = QAM_to_real_waveform(upsampled, CARRIER_FREQ, SAMPLE_RATE);
    log_real_signal(upconverted, "../../log/qam_unwaved.csv"); // Log QAM unwaved

    // Normalize signal
    int_signal normalized = normalize(upconverted);
    log_int_signal(normalized, "../../log/normalized.csv"); // Log normalized signal

    // Save QAM in wav file
    write_wav_real(normalized, "../../wav/qam.wav", SAMPLE_RATE);


    // At this point the QAM_received.wav file should be transmitted to the receiver
    // Further analysis will be done with this file containing the effects of the channel


    //Read QAM from wav file
    int_signal received_signal = read_wav_real("../../wav/qam.wav");
    log_int_signal(received_signal, "../../log/received_signal.csv"); // Log received signal

    // Convert received signal to complex signal
    complex_signal received_complex_signal = real_waveform_to_QAM(received_signal, CARRIER_FREQ, SAMPLE_RATE);
    log_complex_signal(received_complex_signal, "../../log/received_complex_signal.csv"); // Log received complex signal

    // Downsample
    complex_signal downsampled = downsample(received_complex_signal, 160);
    log_complex_signal(downsampled, "../../log/downsampled.csv"); // Log downsampled signal

    // Equalize


    // Apply QAM demodulation
    int_signal demod = demod_QAM(downsampled);
    log_int_signal(demod, "../../log/demod.csv"); // Log demodulated signal
}
