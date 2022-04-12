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
warnings.filterwarnings('ignore')

audio_path = "C:/test/audio/ba_ba.wav"

sample_rate, samples = wavfile.read(audio_path)

import tensorflow as tf
from keras.models import load_model

model = load_model("C:/Users/nandi/phoneme_vowel_or_consonant_cnn")

