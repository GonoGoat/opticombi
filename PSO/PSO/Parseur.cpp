#include "Parseur.h"



std::unordered_map<std::string, Matrice> conversionToEnum = {
    {"D",Dirt},{"Tu",Tank},{"b",Base},{"w",Water},{"Bs",Sollid_Block},
    {"Bm",Movable_Block},{"B",Bricks},{"Au",Anti_Tank_U},{"Ad",Anti_Tank_D},
    {"Al",Anti_Tank_L},{"Ar",Anti_Tank_R},{"Mur",Mirror_UR},{"Mul",Mirror_UL},
    {"Mdr",Mirror_DR},{"Mdl",Mirro_DL},{"Wu",Way_U},{"Wd",Way_D},{"Wr",Way_R},
    {"Wl",Way_L},{"C",Crystal_Block},{"Rur",Rotative_Mirror_UR},{"Rul",Rotative_Mirror_UL},
    {"Rdr",Rotative_Mirror_DR},{"Rdl",Rotative_Mirror_DL},{"I",Ice},{"i",Thin_Ice},
    {"Tr",Tunnel_Red},{"Tg",Tunnel_Green},{"Tb",Tunnel_Blue},{"Tc",Tunnel_Cyan},
    {"Ty",Tunnel_Yellow},{"Tp",Tunnel_Pink},{"Tw",Tunnel_White},{"Td",Tunnel_Dark}
};

void parsage(std::string nom_fichier, int matrice[16][16]) {

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

            for (int i = 0; i < 16; i++) {
                ligne = lignes[index_saut_ligne + i];
                // Supprime les espaces et les caractères de nouvelle ligne éventuels
                while (!ligne.empty() && (ligne.back() == ' ' || ligne.back() == '\n')) {
                    ligne.pop_back();
                }

                // Divise la ligne en éléments
                std::string motActuel = "";
                int j = 0;

                int type_case = 0;

                for (char caractere : ligne) {
                    if (caractere != ' ') {
                        motActuel += caractere; 
                    }
                    else if (!motActuel.empty()) {
                        type_case = conversion(motActuel);
                        matrice[i][j] = type_case;
                        motActuel.clear();
                        j++;
                    }
                }

                if (!motActuel.empty()) {
                    type_case = conversion(motActuel);
                    matrice[i][j] = type_case;
                }
            }

            //end = std::chrono::system_clock::now();
            
            // Affiche la matrice
            for (int i = 0; i < 16; i++) {
                for (int j = 0; j < 16; j++) {
                    std::cout << matrice[i][j] << " ";
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
