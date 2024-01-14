# opticombi
## Description
Projet d'optimisation combinatoire : Optimisation de la réussite jeu du LaserTank par la méthode des essaims particulaires
## Membres
- MANTEAU Maxence
- DORMY Cédric
- VERQUIN Morgan
- COPPENS Thomas
- VERSCHRAEGEN Gauthier
## Ressources
- **GitHub** : https://github.com/GonoGoat/opticombi
- **Clockify** : https://app.clockify.me/shared/651550e0ae8eaa33dbe8c3e2
- **Overleaf** : https://www.overleaf.com/read/ghtwptqvkqzb

## Dossiers
- **bin** : Exécutables du programme - Fichiers *.exe*
- **includes** : Fichiers d'en-tête du code - Fichiers *.h*
- **Maps** : Cartes utilisée par leu jeu LaserTank - Fichiers *.lt4*
- **Output** : Sorties du programme
    - Solution de la carte - Fichiers *.ltr*
    - Représentation graphique de trajectoire si demandé - Fichiers *.svg*
- **src** : Code source du programme - Fichiers *.cpp*

## Exécution du programme
1. Compilation du programme \(g++\) : `g++ -g -I ./includes ./src/*.cpp -o ./bin/pso.exe`
2. Exécution du programme : `./bin/pso.exe [path]` avec **path** comme argument optionnel
    - Si **path** est indiqué et représente un chemin valable vers un fichier *.lt4*, le programme s'appliquera sur cette carte
    - Si **path** n'est pas spécifié, le programme s'exécutera sur une carte déclarée à la **ligne 20 du fichier Main.cpp \(dossier src\)**
3. 2 Paramètres sont demandés en console
    - Le nombre de particule par arrivée \(Nombre entier - Valeur recommandée : <100\)
    - Le nombre maximum d'itération \(Nombre entier\)
4. Collecte des sorties : Le fichier de solution généré pour une carte portera le nom du fichier *.lt4* mais avec l'extension *ltr* dans le dossier **Output**

## Paramétrage de l'algorithme
2 fichiers servent à la configuration de l'algorithme
- `./src/Main.cpp`
- `./includes/data.h`