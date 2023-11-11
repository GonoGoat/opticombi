import sys
import numpy as np  # Importez la bibliothèque NumPy sous l'alias np

# Création d'une matrice vide de 16x16
matrice = np.empty((16, 16), dtype=object)  # dtype=object pour stocker des chaînes de caractères

# Attention de bien remplacer le paramètre par le nom du fichier
nom_fichier = "Beginner-II.lt4"

try:
    with open(nom_fichier, 'r') as fichier:
        lignes = fichier.readlines()
        index_saut_ligne = 0

        #Détecte le saut de ligne vide avant la matrice
        for index, l in enumerate (lignes, start=1):
            if l.isspace():
                index_saut_ligne = index #Stock dans la variable le saut de ligne détecté
                #print(f"Ligne vide détectée à la ligne {index}: '{l.strip()}'")


        for i in range(16):
            ligne = lignes[index_saut_ligne+i].strip()  # Supprime les espaces et les sauts de ligne éventuels
            elements = ligne.split()   # Divise la ligne en éléments
            
            # Assure que la ligne contient suffisamment d'éléments pour remplir la matrice 16x16
            if len(elements) < 16:
                print("La ligne", index_saut_ligne + i + 1, "ne contient pas suffisamment d'éléments.")
                sys.exit(1)
            
            for j in range(16):
                matrice[i][j] = elements[j]  # Stocke chaque élément dans la matrice comme une chaîne de caractères
                
    # Affiche la matrice
    print(matrice)

except FileNotFoundError:
    print("Le fichier", nom_fichier, "n'a pas été trouvé.")
