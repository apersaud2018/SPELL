import matplotlib.pyplot as plt
from scipy import signal
from scipy.io import wavfile
import glob
import tqdm
import numpy as np
from skimage.transform import resize
import pickle
import os
import simpleaudio as sa
import warnings
import librosa
import librosa.display
import sys
warnings.filterwarnings('ignore')

audio_path = "C:/test/audio/ba_ba.wav"

sample_rate, samples = wavfile.read(audio_path)

import tensorflow as tf
from keras.models import load_model

model_vc = load_model("../models/phoneme_vowel_or_consonant_cnn")
model_vowel = load_model("../models/phoneme_vowel_cnn")
model_consonants = load_model("../models/phoneme_consonants_cnn")

path = sys.argv[1]

times = [int(i) for i in sys.argv[2:]]

samples, sample_rate = librosa.load(path)

times.append(len(samples))
X = []
for i,start_sample in enumerate(times[0:len(times)-1]):
    end_sample = times[i+1]
    spectrogram = librosa.feature.melspectrogram(samples[start_sample:end_sample],sample_rate,n_mels=40)
    spectrogram = np.log(spectrogram)
    base_img = resize(spectrogram, (40,20))
    X.append(base_img)
    
X = np.array(X)
X = np.expand_dims(X,axis=-1)
X = X/27

prediction = model_vc.predict(X)
prediction = np.argmax(prediction, axis=1)

#pause = 0, consonant = 1, vowel = 2

with open("../scripts/num_to_vowels.pickle", "rb") as f:
    num_to_vowel = pickle.load(f)
    
with open("../scripts/num_to_consonants.pickle", "rb") as f:
    num_to_consonant = pickle.load(f)

output = []
for i,item in enumerate(prediction):
    if item == 0:
        output.append('pau')
    elif item == 1:
        output.append(num_to_consonant[np.argmax(model_consonants.predict(np.array([X[i]]))[0])])
    elif item == 2:
        output.append(num_to_vowel[np.argmax(model_vowel.predict(np.array([X[i]]))[0])])
print("\n#######")
print(len(output))
for phoneme in output:
    print(phoneme)

