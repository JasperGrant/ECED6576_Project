// QAM.h
// Jasper Grant
// Script to take a Gold Sequence, apply QAM and then transmit

#include <stdio.h>
#include <complex.h>
#include "QAM.h"

int main(void) {
   // Generate gold code
   int_signal gold = generate_gold_code(11, 0, 2048, 2047);

   // Apply QAM
   complex_signal qam = QAM(gold);
}
