// QAM.h
// Jasper Grant
// Script to take a Gold Sequence, apply QAM and then transmit

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

   // To get about 600 QAM symbols a second we need about 80 gold signals

   int_signal repeated_gold = repeat_signal(gold, 24);

   // Log gold code
   log_int_signal(repeated_gold, "../../log/gold_code.csv");

   // Apply QAM and carrier freq
   real_signal qam = mod_QAM(repeated_gold, CARRIER_FREQ, SYMBOL_RATE);

   // Log QAM
   log_real_signal(qam, "../../log/qam.csv");

   // Upsample
   // real_signal upsampled_qam = upsample(qam, SAMPLE_RATE / SYMBOL_RATE);

   // Save QAM in wav file
   write_wav_real(qam, "../../wav/qam.wav", SAMPLE_RATE, SAMPLE_RATE / SYMBOL_RATE);

   real_signal qam_unwaved = read_wav_real("../../wav/qam.wav", SAMPLE_RATE / SYMBOL_RATE);

   log_real_signal(qam_unwaved, "../../log/qam_unwaved.csv");

   // Transmit QAM
   // Simulate channel with model

   // Apply low pass filter
   // real_signal low_pass = low_pass_filter(qam, CARRIER_FREQ, SAMPLE_RATE);

   // Apply QAM demodulation and downsample
   int_signal demod = demod_QAM(qam, CARRIER_FREQ, SAMPLE_RATE);

   // Log demodulated signal
   log_int_signal(demod, "../../log/demod.csv");

   // Equalize somewhere in here
}
