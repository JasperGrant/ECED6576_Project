# Python file to verify correctness of wav files
# Jasper Grant

import wave
import array

wav_file = wave.open("wav/qam.wav", "rb")

wav_file_data = array.array("d", wav_file.readframes(wav_file.getnframes()))

print(wav_file_data)
