# Python file to verify correctness of wav files
# Jasper Grant

import wave
import array
import matplotlib.pyplot as plt
import numpy as np

wav_file = wave.open("wav/qam.wav", "rb")

wav_file_data = array.array("h", wav_file.readframes(wav_file.getnframes()))
fs = wav_file.getframerate()

times = np.arange(len(wav_file_data)) / fs


# Plot the data
plt.figure()
plt.plot(times, wav_file_data)
plt.title("QAM Signal")
plt.xlabel("Time (s)")
plt.ylabel("Amplitude")

# Plot the FFT
plt.figure()
plt.magnitude_spectrum(wav_file_data, Fs=fs, scale="dB")
plt.title("QAM Signal FFT")
plt.xlabel("Frequency (Hz)")
plt.ylabel("Magnitude (dB)")

plt.show()
