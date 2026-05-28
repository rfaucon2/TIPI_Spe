import matplotlib.pyplot as plt 
import numpy as np 
import csv
from os import system as execute

with open("BH_record.csv", "r+") as rec:
    data = np.array(list(csv.reader(rec)))
    print(data[:, 1])

