#include "Detection_Start_End.h"
/**
 * @brief Détecte le point de départ et les points d'arrivées d'une carte
 *
 * @param mapParams Les paramètres de la carte
 */

void detection(mapStruct* mapParams) {
    for (int i = 0; i < mapParams->nbr_lignes; i++) { // Pour chaque ligne
        for (int j = 0; j < mapParams->nbr_colonnes; j++) { // Pour chaque colonne
            //std::cout << j;
            if (mapParams->matrice_fixe[i][j] == Tank) {
                mapParams->Origine_x = j;
                mapParams->Origine_y = i;
                // std::cout << "Depart trouve : " << j << "  " << i;
            }
            else if (mapParams->matrice_fixe[i][j] == Base) {
                mapParams->Finish_x.push_back(j);
                mapParams->Finish_y.push_back(i);
                mapParams->nbr_arrive += 1;
                // std::cout << "Arrive trouve : " << j << "  " << i;
            }
        }
        //std::cout<< std::endl;
    }
}