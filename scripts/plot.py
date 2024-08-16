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


def plot_real_and_complex_signal(file, title):
    signal = [
        complex(float(line.strip().split(",")[0]), float(line.strip().split(",")[1]))
        for line in open(file, "r")
    ]
    plt.figure()
    plt.plot([value.real for value in signal])
    plt.title(title)
    plt.xlabel("Sample")
    plt.ylabel("Amplitude")
    plt.figure()
    plt.plot([value.imag for value in signal])
    plt.title(title)
    plt.xlabel("Sample")
    plt.ylabel("Amplitude")


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
    correlation = abs(np.correlate(signal, reference, mode="full"))
    # Normalize
    correlation = correlation / np.max(correlation)
    plt.figure()
    plt.plot(correlation)
    plt.title(title)
    plt.xlabel("Sample")
    plt.ylabel("Amplitude")


def plot_fft(file, title):
    signal = [float(line.strip()) for line in open(file, "r")]
    fft = np.fft.fft(signal)
    plt.figure()
    plt.plot(np.abs(fft))
    plt.title(title)
    plt.xlabel("Frequency")
    plt.ylabel("Magnitude")


def plot_magnitude_spectrum(file, title):
    signal = [float(line.strip()) for line in open(file, "r")]
    plt.figure()
    plt.magnitude_spectrum(signal, scale="dB")
    plt.title(title)
    plt.xlabel("Frequency")
    plt.ylabel("Magnitude")


def normalize_and_plot_constellation(file, title):
    signal = [
        complex(float(line.strip().split(",")[0]), float(line.strip().split(",")[1]))
        for line in open(file, "r")
    ]
    max_value = max([abs(value) for value in signal])
    signal = [value / max_value for value in signal]
    plt.figure()
    plt.scatter([value.real for value in signal], [value.imag for value in signal])
    plt.title(title)
    plt.xlabel("Real")
    plt.ylabel("Imaginary")


def compare_plots(filesnames):
    fig, axs = plt.subplots(len(filesnames), 1)
    for i, file in enumerate(filesnames):
        signal = [float(line.strip()) for line in open(file, "r")]
        axs[i].plot(signal)
        axs[i].set_title(file)
        axs[i].set_xlabel("Sample")
        axs[i].set_ylabel("Amplitude")

    # Calculate error between the first signal and the rest
    signal = [float(line.strip()) for line in open(filesnames[0], "r")]
    for i, file in enumerate(filesnames[1:]):
        compare_signal = [float(line.strip()) for line in open(file, "r")]
        error = np.sum(np.abs(np.array(signal) - np.array(compare_signal)))
        print(f"Error between {filesnames[0]} and {file}: {error}")


if __name__ == "__main__":

    # plot_autocorrelation("log/gold_code_ref.csv", "Gold Code Autocorrelation")

    # plot_correlation_to_reference(
    #     "log/gold_code.csv",
    #     "log/gold_code_ref.csv",
    #     "Tx Correlation with initial Gold Sequence",
    # )
    # normalize_and_plot_constellation("log/qam.csv", "Gold Code Constellation")
    # normalize_and_plot_constellation(
    #     "log/pulse_shaped.csv", "Gold Code Ref Constellation"
    # )
    # normalize_and_plot_constellation("log/upsampled.csv", "QAM Constellation")
    normalize_and_plot_constellation("log/downsampled.csv", "QAM Constellation")

    # plot_correlation_to_reference(
    #     "log/demod.csv",
    #     "log/gold_code_ref.csv",
    #     "Demodulated Signal Correlation with Gold Sequence",
    # )

    # compare_plots(
    #     [
    #         "log/gold_code_ref.csv",
    #         "log/equalization_results/before.csv",
    #         "log/equalization_results/after.csv",
    #     ]
    # )

    # plot_correlation_to_reference(
    #     "log/equalization_results/after.csv",
    #     "log/gold_code_ref.csv",
    #     "Correlation to Gold Sequence after Equalization",
    # )

    # plot_real_signal("log/received_signal.csv", "Received Signal")

    # plot_real_signal("log/channel_impulse_response.csv", "Channel Impulse Response")

    # plot_real_signal("log/gold_code.csv", "Gold Sequence")

    # plot_real_and_complex_signal("log/qam.csv", "QAM Signal")

    # plot_real_signal("filters/stage_1.csv", "Stage 1 Filter")

    # plot_real_signal("filters/stage_2.csv", "Stage 2 Filter")

    # plot_real_and_complex_signal("log/pulse_shaped.csv", "Pulse shaped QAM Signal")

    # plot_real_and_complex_signal("log/upsampled.csv", "Upsampled QAM Signal")

    # # plot_complex_signal("log/upsampled.csv", "Upsampled QAM Signal")

    # plot_real_signal("log/qam_unwaved.csv", "Signal going int wav file")

    # plot_fft("log/qam_unwaved.csv", "FFT of QAM Signal")

    plt.show()
