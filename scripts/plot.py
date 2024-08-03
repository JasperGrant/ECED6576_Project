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


if __name__ == "__main__":

    plot_real_signal("log/gold_code.csv", "Gold Code")

    plot_autocorrelation("log/gold_code.csv", "Gold Code Autocorrelation")

    plot_complex_signal("log/qam.csv", "QAM Signal")

    plt.show()
