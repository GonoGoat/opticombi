#include "Parseur.h"

/**
 * @brief Convertit un fichier .lt4 en une matrice exploitable par le programme
 * 
 * @param mapParams Données de la carte
 */
void parsage(mapStruct* mapParams) {
    /*std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();*/
    // Cr�e un tableau � 2D pour stocker la matrice

    try {
        std::ifstream fichier(mapParams->nom_fichier);
        if (fichier.is_open()) {
            std::vector<std::string> lignes;
            std::string ligne;

            // Lis toutes les lignes du fichier dans un vecteur
            while (std::getline(fichier, ligne)) {
                lignes.push_back(ligne);
            }

            int index_saut_ligne = 0;

            // D�tecte le saut de ligne vide avant la matrice
            for (int index = 0; index < lignes.size(); index++) {
                if (lignes[index].empty()) {
                    index_saut_ligne = index + 1; // Stocke l'index du saut de ligne
                    //std::cout << "Ligne vide d�tect�e � la ligne " << index + 1 << ": '" << lignes[index] << "'" << std::endl;
                }
            }

            // D�tection des dimensions de la matrice
            if (lignes[0].find("Rows: ") != std::string::npos) {
                mapParams->nbr_lignes = std::stoi(lignes[0].substr(6, lignes.size()));
            }
            else {
                throw std::runtime_error("Nombres de lignes non detectees sur le fichier .lt4");
            }
            if (lignes[1].find("Cols: ") != std::string::npos) {
                mapParams->nbr_colonnes = std::stoi(lignes[1].substr(6, lignes.size()));
            }
            else {
                throw std::runtime_error("Nombres de colonnes non detectees sur le fichier .lt4");
            }

            for (int i = 0; i < mapParams->nbr_lignes; i++) {
                ligne = lignes[index_saut_ligne + i];
                // Supprime les espaces et les caract�res de nouvelle ligne �ventuels
                while (!ligne.empty() && (ligne.back() == ' ' || ligne.back() == '\n')) {
                    ligne.pop_back();
                }

                // Divise la ligne en �l�ments
                std::string motActuel = "";
                int j = 0;

                int type_case = 0;
                std::vector<int> row;
                std::vector<int> row_mobile;

                for (char caractere : ligne) {
                    if (caractere != ' ') {
                        motActuel += caractere;
                    }
                    else if (!motActuel.empty()) {
                        type_case = conversion(motActuel);
                        if ((type_case >= Movable_Block && type_case <= Mirror_DL) || (type_case >= Rotative_Mirror_UR && type_case <= Rotative_Mirror_DL) || type_case == Thin_Ice) {
                            row_mobile.push_back(type_case);
                            row.push_back(Dirt);
                        }
                        else {
                            row_mobile.push_back(Dirt);
                            row.push_back(type_case);
                        }
                        motActuel.clear();
                        j++;
                    }
                }


                if (!motActuel.empty()) {
                    type_case = conversion(motActuel);
                    if ((type_case >= Movable_Block && type_case <= Mirror_DL) || (type_case >= Rotative_Mirror_UR && type_case <= Rotative_Mirror_DL) || type_case == Thin_Ice) {
                        row_mobile.push_back(type_case);
                        row.push_back(Dirt);
                    }
                    else {
                        row_mobile.push_back(Dirt);
                        row.push_back(type_case);
                    }
                }
                mapParams->matrice_fixe.push_back(row);
                mapParams->matrice_mobile.push_back(row_mobile);
            }

            //end = std::chrono::system_clock::now();

            // Affiche la matrice fixe
            std::cout << "Matrice fixe" << std::endl;
            for (int i = 0; i < mapParams->nbr_lignes; i++) {
                for (int j = 0; j < mapParams->nbr_colonnes; j++) {
                    std::cout << mapParams->matrice_fixe[i][j] << " ";
                }
                std::cout << std::endl;
            }
            //Affiche la matrice mobile
            std::cout << "Matrice mobile" << std::endl;
            for (int i = 0; i < mapParams->nbr_lignes; i++) {
                for (int j = 0; j < mapParams->nbr_colonnes; j++) {
                    std::cout << mapParams->matrice_mobile[i][j] << " ";
                }
                std::cout << std::endl;
            }

            /*long long int microseconde = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            std::cout << "Temp du parsage " << microseconde << " microsec" << std::endl << std::endl;*/

        }
        else {
            std::cerr << "Le fichier " << mapParams->nom_fichier << " n'a pas ete trouve." << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }
}

/**
 * @brief Conversion d'une chaine de caract�re lue sur le fichier .lt4 en un bloc reconnu
 *
 * @param caractere IN - La chaien de caract�re � reconnaitre comme bloc
 * @return Matrice - Le bloc reconnu
 */
Matrice conversion(const std::string& caractere)
{
    if (conversionToEnum.find(caractere) != conversionToEnum.end()) {
        return conversionToEnum[caractere];
    }
    return Matrice(-1);
}