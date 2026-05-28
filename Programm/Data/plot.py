import matplotlib.pyplot as plt 
import numpy as np 
import csv
from os import system as execute

def to_int_list(l):
    out = []
    for v in l:
        out.append(float(v))
    return out

def average(l):
    s = 0 
    for v in l:
        s+=v
    return s/len(l)
# Generate data
def gen_cmp_N_BH(theta):
    N_list = np.logspace(1, 5, num = 15, base=10.0)
    Naive_record_list = []
    BH_record_list = []
    seed = 980987
    # Generate data
    for N in [10, 20]:
        execute(f"Simulation N {int(N)} {theta} {seed} N_record.csv")
        with open("N_record.csv", "r+") as N_record:
            Naive_record_list = np.array(list(csv.reader(N_record)))
        
        execute(f"Simulation BH {N} {theta} {seed} BH_record.csv")
        with open("BH_record.csv", "r+") as BH_record:
            BH_record_list = np.array(list(csv.reader(BH_record)))
    # calculate average fps
    N_avg_list  = []
    BH_avg_list = []
    frame_count = len(Naive_record_list[0])

    for i in range(len(BH_record_list)):
        N_avg_list.append( average(to_int_list(Naive_record_list[i])))
        BH_avg_list.append( average(to_int_list(BH_record_list[i])))
    
    print(theta)
    with open(f"record_theta_{theta}.csv", "w+") as output:
        for i in range(len(N_list)):
            output.write(f"{int(N_list[i])}, {N_avg_list[i]}, {BH_avg_list[i]}\n")
            print(f"{int(N_list[i])}, {N_avg_list[i]}, {BH_avg_list[i]}")

def display_data(theta):
    N_list = []
    N_avg_list = []
    BH_avg_list = []

    with open(f"record_theta_{theta}.csv", "r+") as file:
        rec = list(csv.reader(file))
        print(rec)
        for c in rec:
            N_list.append(int(c[0]))
            N_avg_list.append(float(c[1]))
            BH_avg_list.append(float(c[2]))
    
    print(N_avg_list)
    print(BH_avg_list)

    plt.plot(N_list, N_avg_list)
    plt.plot(N_list, BH_avg_list)

    plt.xlabel("N(Nombre d'etoile)")
    plt.ylabel(f"Fps moyen sur 500 images")

    plt.show()

for theta in [1]:
    gen_cmp_N_BH(theta)

display_data(1)
