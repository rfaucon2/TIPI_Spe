import matplotlib.pyplot as plt 
import numpy as np 
import csv
from os import system as execute
from random import randint

def to_float_list(l):
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
    N_list = np.logspace(1, 3.5, num = 10, base=10.0)
    N_avg_list  = []
    BH_avg_list = []
    Tries = 5
    # Génère des données à théta constant et N variable pour les deux algorithmes
    for N in N_list:
        print(f"{theta=}, {N=}")
        N_avg = []
        BH_avg = []
        # Execute plusieurs simulations pour chaque algorithme et moyenne les temps de calcul
        for i in range(Tries):
            seed = randint(0, 100000) 
            execute(f"Simulation N {int(N)} {theta} {seed} N_record.csv")
            with open("N_record.csv", "r+") as N_record:
                N_record_list = np.array(list(csv.reader(N_record)))[:,1]
                N_avg.append(average(to_float_list(N_record_list)))
                
        for i in range(Tries):
            seed = randint(0, 100000) 
            execute(f"Simulation BH {N} {theta} {seed} BH_record.csv")
            with open("BH_record.csv", "r+") as BH_record:
                BH_record_list = np.array(list(csv.reader(BH_record)))[:,1]
                BH_avg.append(average(to_float_list(BH_record_list)))
        
        N_avg_list.append(average(N_avg))
        BH_avg_list.append(average(BH_avg))

    # Enregistre les données dans un fichier csv pour les afficher plus tard
    with open(f"record_theta_{theta}.csv", "w+") as output:
        for i in range(len(N_list)):
            output.write(f"{int(N_list[i])}, {N_avg_list[i]}, {BH_avg_list[i]}\n")

def display_cmp(theta):
    N_list = []
    N_avg_list = []
    BH_avg_list = []

    # Récupère les données du fichier csv
    with open(f"record_theta_{theta}.csv", "r+") as file:
        rec = list(csv.reader(file))
        print(rec)
        for c in rec:
            N_list.append(int(c[0]))
            N_avg_list.append(float(c[1]))
            BH_avg_list.append(float(c[2]))

    # Affiche les données
    plt.plot(N_list, N_avg_list, label="Algorithme naif")
    plt.plot(N_list, BH_avg_list, label=f"Barnes-Hut ($theta$ = {theta})")
    plt.legend()
    plt.xlabel("N(Nombre d'etoile)")
    plt.ylabel(f"Temps moyen de calcule d'une image sur 100 images (s)")
    plt.show()

def gen_naive():
    N_list = np.logspace(2, 4, num = 10, base=10.0)
    N_avg_list  = []
    # Génère des données à théta constant et N variable pour l'algorithme naïf
    for N in N_list:
        print(f"{N=}")
        N_avg = []
        seed = randint(0, 100000) 
        execute(f"Simulation N {int(N)} 0.1 {seed} N_record.csv")
        with open("N_record.csv", "r+") as N_record:
            N_record_list = np.array(list(csv.reader(N_record)))[:,1]
            N_avg.append(average(to_float_list(N_record_list)))
        N_avg_list.append(average(N_avg))

    # Enregistre les données dans un fichier csv pour les afficher plus tard
    with open(f"record_naive.csv", "w+") as output:
        for i in range(len(N_list)):
            output.write(f"{int(N_list[i])}, {N_avg_list[i]}\n")

def display_naive():
    N_list = []
    N_avg_list = []
    with open(f"record_naive.csv", "r+") as file:
        rec = list(csv.reader(file))
        print(rec)
        for c in rec:
            N_list.append(int(c[0]))
            N_avg_list.append(float(c[1]))
    
    # Affiche les données
    plt.plot(N_list,N_avg_list, "-x")
    plt.xlabel("N(Nombre d'etoile)")
    plt.ylabel(f"Temps moyen de calcule d'une image sur 100 images (s)")
    plt.grid(True)
    plt.show()

def gen_BH_N():
    N_list = np.logspace(2, 5, 15)
    data_list  = []
    # Génère des données à théta constant et N variable pour l'algorithme de Barnes-Hut
    for N in N_list:
        print(f"{N=}")
        seed = randint(0, 100000) 
        execute(f"Simulation BH {N} 1.0 {seed} N_record.csv")
        with open("N_record.csv", "r+") as N_record:
            N_record_list = np.array(list(csv.reader(N_record)))[:,1]
            data_list.append(average(to_float_list(N_record_list)))

    # Enregistre les données dans un fichier csv pour les afficher plus tard
    with open(f"record_t.csv", "w+") as output:
        for i in range(len(N_list)):
            output.write(f"{N_list[i]}, {data_list[i]}\n")

def display_BH_N():
    t_list = []
    t_avg_list = []

    # Récupère les données du fichier csv
    with open(f"record_t.csv", "r+") as file:
        rec = list(csv.reader(file))
        for c in rec:
            t_list.append(float(c[0]))
            t_avg_list.append(float(c[1]))
    
    # Affiche les données
    plt.xlabel("N")
    plt.plot(t_list, t_avg_list/(t_list), label="T(N)/N")
    plt.plot(t_list, t_avg_list/(t_list)*np.log(t_list), label="T(N)/NlogN")
    plt.ylabel(f"Temps de calcul moyen sur 100 images (s)")
    plt.title("Theta = 1.0")
    plt.grid(True)
    plt.show() 
