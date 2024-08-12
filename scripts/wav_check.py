# Python file to verify correctness of wav files
# Jasper Grant

import wave
import array
import matplotlib.pyplot as plt

wav_file = wave.open("wav/qam.wav", "rb")

wav_file_data = array.array("d", wav_file.readframes(wav_file.getnframes()))

# Plot the data
plt.figure()
plt.plot(wav_file_data)
plt.title("Wav File Data")
plt.xlabel("Sample")
plt.ylabel("Amplitude")
plt.show()
