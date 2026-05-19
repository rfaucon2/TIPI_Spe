import matplotlib.pyplot as plt 
import numpy as np 
import csv

with open("BH_record.csv", "r+") as data_file:
    data = list(csv.reader(data_file))[1:]
    x = []
    y = []
    t = []
    for line in data:
        x.append(line[1])
        y.append(line[2])
        t.append(line[0])

    x = np.array(x)
    y = np.array(y)
    t = np.array(t)
    
    plt.figure()
    plt.xlabel("X")
    plt.ylabel("y")

    plt.plot(x, y)

        
    plt.show()
