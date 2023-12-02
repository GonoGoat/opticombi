#include "Parseur.h"

void parsage(std::string nom_fichier, std::vector<std::vector<int>>* matrice, int* nombreLignes, int* nombreColonnes) {

    /*std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();*/
    // Crée un tableau à 2D pour stocker la matrice

    try {
        std::ifstream fichier(nom_fichier);
        if (fichier.is_open()) {
            std::vector<std::string> lignes;
            std::string ligne;

            // Lis toutes les lignes du fichier dans un vecteur
            while (std::getline(fichier, ligne)) {
                lignes.push_back(ligne);
            }

            int index_saut_ligne = 0;

            // Détecte le saut de ligne vide avant la matrice
            for (int index = 0; index < lignes.size(); index++) {
                if (lignes[index].empty()) {
                    index_saut_ligne = index + 1; // Stocke l'index du saut de ligne
                    //std::cout << "Ligne vide détectée à la ligne " << index + 1 << ": '" << lignes[index] << "'" << std::endl;
                }
            }

            // Détection des dimensions de la matrice
            if (lignes[0].find("Rows: ") != std::string::npos) {
                *nombreLignes = std::stoi(lignes[0].substr(6,lignes.size()));
            }
            else {
                throw std::runtime_error("Nombres de lignes non detectees sur le fichier .lt4");
            }
            if (lignes[1].find("Cols: ") != std::string::npos) {
                *nombreColonnes = std::stoi(lignes[1].substr(6,lignes.size()));
            }
            else {
                throw std::runtime_error("Nombres de colonnes non detectees sur le fichier .lt4");
            }

            for (int i = 0; i < *nombreLignes; i++) {
                ligne = lignes[index_saut_ligne + i];
                // Supprime les espaces et les caractères de nouvelle ligne éventuels
                while (!ligne.empty() && (ligne.back() == ' ' || ligne.back() == '\n')) {
                    ligne.pop_back();
                }

                // Divise la ligne en éléments
                std::string motActuel = "";
                int j = 0;

                int type_case = 0;
                std::vector<int> row;

                for (char caractere : ligne) {
                    if (caractere != ' ') {
                        motActuel += caractere; 
                    }
                    else if (!motActuel.empty()) {
                        type_case = conversion(motActuel);
                        row.push_back(type_case);
                        motActuel.clear();
                        j++;
                    }
                }

                if (!motActuel.empty()) {
                    type_case = conversion(motActuel);
                    row.push_back(type_case);
                }
                matrice->push_back(row);
            }

            //end = std::chrono::system_clock::now();
            
            // Affiche la matrice
            for (int i = 0; i < *nombreLignes; i++) {
                for (int j = 0; j < *nombreColonnes; j++) {
                    std::cout << (*matrice)[i][j] << " ";
                }
                std::cout << std::endl;
            }

            /*long long int microseconde = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            std::cout << "Temp du parsage " << microseconde << " microsec" << std::endl << std::endl;*/

        }
        else {
            std::cerr << "Le fichier " << nom_fichier << " n'a pas ete trouve." << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }
}

Matrice conversion(const std::string& caractere)
{
    if (conversionToEnum.find(caractere) != conversionToEnum.end()) {
        return conversionToEnum[caractere];
    }
    return Matrice(-1);
}