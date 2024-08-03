// QAM.h
// Jasper Grant
// Script to take a Gold Sequence, apply QAM and then transmit

#include "QAM.h"
#include "log.h"

int main(void)
{
   // Generate gold code
   int_signal gold = generate_gold_code(11, 0, 2048, 2047);

   // Log gold code
   log_int_signal(gold, "../../log/gold_code.csv");

   // Upsample gold code

   // Log upsampled gold code

   // Apply QAM
   complex_signal qam = QAM(gold);

   // Log QAM
   log_complex_signal(qam, "../../log/qam.csv");

   // Transmit QAM
   // Simulate channel with model

   // Apply QAM demodulation

   // Downsample QAM

   // Equalize somewhere in here
}
