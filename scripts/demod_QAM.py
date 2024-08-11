# Python script written to debug QAM generation and demodulation
# Jasper Grant

import numpy as np
import matplotlib.pyplot as plt


def demod_QAM(signal, carrier_freq, sample_rate):
    # Generate carrier signal
    carrier = np.exp(
        1j * 2 * np.pi * carrier_freq / sample_rate * np.arange(len(signal))
    )
    # Demodulate signal
    demodulated = signal * carrier
    # Plot demodulated signal
    plt.figure()
    plt.plot(demodulated)
    plt.title("Demodulated Signal")
    plt.xlabel("Sample")
    plt.ylabel("Amplitude")

    # Plot autocorrelation
    autocorrelation = np.correlate(demodulated, demodulated, mode="full")

    # Normalize
    autocorrelation = autocorrelation / np.max(autocorrelation)
    plt.figure()

    plt.plot(autocorrelation)

    plt.title("Demodulated Signal Autocorrelation")
    plt.xlabel("Sample")
    plt.ylabel("Amplitude")
    plt.show()


def main():
    signal = [float(line.strip()) for line in open("log/qam.csv", "r")]

    demod_QAM(signal, 8000, 96000)


if __name__ == "__main__":
    main()
