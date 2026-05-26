import matplotlib.pyplot as plt 
import numpy as np 
import csv

with open("BH_record.csv", "r+") as data_file:
    with open("Naive_record.csv", "r+") as sec_file:
        data = list(csv.reader(data_file))[1:]
        data_2 = list(csv.reader(sec_file))[1:]
        x = []
        y = []
        t = []
        for i in range(len(data)):
            x.append(float(data[i][1])-float(data_2[i][1]))
            y.append(float(data[i][2])-float(data_2[i][2]))
            t.append(int(data[i][0]))

        x = np.array(x)
        y = np.array(y)
        t = np.array(t)
        
        plt.figure()
        plt.xlabel("X")
        plt.ylabel("y")

        plt.plot(t, np.sqrt(x**2 + y**2)) 
        plt.show()
