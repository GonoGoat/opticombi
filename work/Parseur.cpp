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

            // Détection nom de la carte
            if (lignes[2].find("Name: ") != std::string::npos) {
                mapParams->nom_map =lignes[2].substr(6, lignes.size());
            }
            else {
                throw std::runtime_error("Nom de carte non detecte sur le fichier .lt4");
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

            position_valide(mapParams);
            /*for (int i = 0; i < mapParams->nbr_case_ok; i++) {
                std::cout << "Position n " << i << " (" << mapParams->pos_OK_x[i] << "; " << mapParams->pos_OK_y[i] << ")" << std::endl;
            }*/
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

/**
 * @brief Recherche de case pouvant etre utilise comme depart
 *
 * @param mapParams Données de la carte
 */
void position_valide(mapStruct* mapParams)
{
    int case_verif;
    moveStruct verif_case;
    mapParams->nbr_case_ok = 0;

    for (int i = 0; i < mapParams->nbr_lignes; i++) {
        for (int j = 0; j < mapParams->nbr_colonnes; j++) {
            case_verif = mapParams->matrice_mobile[i][j] + mapParams->matrice_fixe[i][j];
            if (case_verif == Dirt) {
                verif_case.depl_y = i;
                verif_case.depl_x = j;
                Verification_Anti_Tank(mapParams, &verif_case);
            }
        }
    }
}

/**
 * @brief Verification que la case n'est pas situe dans la ligne de tire d'un anti tank
 *
 * @param mapParams Données de la carte
 * @param moveParams Données de la position sur la carte
 */
void Verification_Anti_Tank(mapStruct* mapParams, moveStruct* moveParams)
{
    int position = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] + mapParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x];
    bool case_ok = 1;
    //Verification horizontal
        //A droite de la position en cours
    for (int i = 1; i < mapParams->nbr_colonnes - moveParams->depl_x && case_ok == 1; i++) {
        position = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x + i] + mapParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x + i];
        if (position == Sollid_Block || position == Crystal_Block || (position >= Movable_Block && position <= Anti_Tank_R) || (position >= Anti_Tank_Dead_U && position <= Rotative_Mirror_DL)) {
            break;
        }
        else if (position == Anti_Tank_L) {
            case_ok = 0;
            break;
        }
    }

    //A gauche de la position en cours
    for (int i = 1; i < moveParams->depl_x + 1 && case_ok == 1; i++) {
        position = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x - i] + mapParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x - i];
        if (position == Sollid_Block || position == Crystal_Block || (position >= Movable_Block && position <= Anti_Tank_D) || (position >= Anti_Tank_L && position <= Rotative_Mirror_DL)) {
            break;
        }
        else if (position == Anti_Tank_R) {
            case_ok = 0;
            break;
        }
    }

    //Verification vertical
        //En dessous de la position en cours
    for (int i = 1; i < mapParams->nbr_colonnes - moveParams->depl_y && case_ok == 1; i++) {
        position = mapParams->matrice_fixe[moveParams->depl_y + i][moveParams->depl_x] + mapParams->matrice_mobile[moveParams->depl_y + i][moveParams->depl_x];
        if (position == Sollid_Block || position == Crystal_Block || (position >= Movable_Block && position <= Bricks) || (position >= Anti_Tank_D && position <= Rotative_Mirror_DL)) {
            break;
        }
        else if (position == Anti_Tank_U) {
            case_ok = 0;
            break;
        }
    }

    //Au dessus de la position en cours
    for (int i = 1; i < moveParams->depl_y + 1 && case_ok == 1; i++) {
        position = mapParams->matrice_fixe[moveParams->depl_y - i][moveParams->depl_x] + mapParams->matrice_mobile[moveParams->depl_y - i][moveParams->depl_x];
        if (position == Sollid_Block || position == Crystal_Block || (position >= Movable_Block && position <= Anti_Tank_U) || (position >= Anti_Tank_R && position <= Rotative_Mirror_DL)) {
            break;
        }
        else if (position == Anti_Tank_D) {
            case_ok = 0;
            break;
        }
    }

    //Si encore en vie alors deplacement effectué
    if (case_ok == 1) {
        mapParams->nbr_case_ok += 1;
        mapParams->pos_OK_x.push_back(moveParams->depl_x);
        mapParams->pos_OK_y.push_back(moveParams->depl_y);
    }
}
