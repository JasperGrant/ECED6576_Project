# Python file to plot generated CSV signal files
# Jasper Grant

import matplotlib.pyplot as plt
import numpy as np


def plot_real_signal(file, title):
    signal = [float(line.strip()) for line in open(file, "r")]
    plt.figure()
    plt.plot(signal)
    plt.title(title)
    plt.xlabel("Sample")
    plt.ylabel("Amplitude")


def plot_complex_signal(file, title):
    signal = [
        complex(float(line.strip().split(",")[0]), float(line.strip().split(",")[1]))
        for line in open(file, "r")
    ]
    plt.figure()
    plt.scatter([value.real for value in signal], [value.imag for value in signal])
    plt.title(title)
    plt.xlabel("Real")
    plt.ylabel("Imaginary")


def plot_autocorrelation(file, title):
    signal = [float(line.strip()) for line in open(file, "r")]
    autocorrelation = np.correlate(signal, signal, mode="full")
    # Normalize
    autocorrelation = autocorrelation / np.max(autocorrelation)
    plt.figure()
    plt.plot(autocorrelation)
    plt.title(title)
    plt.xlabel("Sample")
    plt.ylabel("Amplitude")


def plot_correlation_to_reference(file, ref, title):
    signal = [float(line.strip()) for line in open(file, "r")]
    reference = [float(line.strip()) for line in open(ref, "r")]
    correlation = np.correlate(signal, reference, mode="full")
    # Normalize
    correlation = correlation / np.max(correlation)
    plt.figure()
    plt.plot(correlation)
    plt.title(title)
    plt.xlabel("Sample")
    plt.ylabel("Amplitude")


if __name__ == "__main__":

    plot_autocorrelation("log/gold_code.csv", "Gold Code Autocorrelation")

    plot_autocorrelation("log/demod.csv", "Demodulated Signal Autocorrelation")

    plot_real_signal("log/gold_code.csv", "Gold Sequence")

    plot_correlation_to_reference(
        "log/demod.csv",
        "log/gold_code_ref.csv",
        "Demodulated Signal Correlation to Reference",
    )

    plt.show()
