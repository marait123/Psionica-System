from random import random
import numpy as np
import pandas as pd
import os
import mne
import re
import pywt
from sklearn.model_selection import train_test_split
from scipy.signal import butter, sosfilt
from joblib import dump, load
from sklearn.utils import shuffle


def read_file(directory, filename):
  x = np.load(f"{directory}/{filename}X.npy")
  y = np.load(f"{directory}/{filename}Y.npy")
  return x, y

# fileno is the number of subject
# def read_file_sim(fileno): # For Simulator
#   labels = {1: "L", 2: "R", 3: "F", 4: "B"}
#   i = fileno
#   x = np.load(f"numpy_test_data/{i}X.npy")
#   y = np.load(f"numpy_test_data/{i}Y.npy")

#   y = np.array([labels[i] for i in y])

#   return x, y

def read_file_sim(directory, filename):
  labels = {1: "L", 2: "R", 3: "F", 4: "B"}
  x = np.load(f"{directory}/{filename}X.npy")
  y = np.load(f"{directory}/{filename}Y.npy")
  y = np.array([labels[i] for i in y])

  return x, y


# X is data of one sample
model = load("model.joblib")
csp = load("csp.joblib")
def predict(x):
  global model
  global csp
  labels = {1: "L", 2: "R", 3: "F", 4: "B"}
  x = [x]
  test_coeff = featurize(x)
  coeff_len = len(test_coeff)
  
  X_test_f = np.concatenate(tuple(csp[j].transform(test_coeff[j]) for j in range(coeff_len)), axis=-1)
  return labels[model.predict(X_test_f[0:1])[0]]

# X is data of one sample
idle_model = load("idle_model.joblib")
idle_csp = load("idle_csp.joblib")
def predict_idle(x):
  global idle_model
  global idle_csp
  
  x = [x]
  test_coeff = featurize(x)
  coeff_len = len(test_coeff)
  
  x_test_f = np.concatenate(tuple(idle_csp[j].transform(test_coeff[j]) for j in range(coeff_len)), axis=-1)
  return idle_model.predict(x_test_f[0:1])[0]


# apply discrete wavelet transform
def featurize(x):
  coeff = pywt.wavedec(x, 'db4', level = 7)
  return coeff  

