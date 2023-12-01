#include "Detection_Start_End.h"
/**
 * @brief Détecte le point de départ et les points d'arrivée d'une carte
 *
 * @param matrice Carte parsée
 * @param nombreLignes Nombres de ligne de la matrice
 * @param nombreColonnes Nombres de colonnes de la matrice
 * @param posOriginX RES - Coordonnée X du point de départ
 * @param posOriginY RES - Coordonnée Y du point de départ
 * @param posFinishX RES - Coordonnées X des arrivées détectées
 * @param posFinishY RES - Coordonnées Y des arrivées Détectées
 * @param nbr_arrive RES - Nombre d'arrivées détectées
 */

void detection(std::vector<std::vector<int>> matrice, int nombreLignes, int nombreColonnes, int* posOriginX, int* posOriginY, std::vector<int>* posFinishX, std::vector<int>* posFinishY, int* nbr_arrive) {
    for (int i = 0; i < nombreLignes; i++) { // Pour chaque ligne
        for (int j = 0; j < nombreColonnes; j++) { // Pour chaque colonne
            //std::cout << j;
            if (matrice[i][j] == Tank) {
                *posOriginX = j;
                *posOriginY = i;
                // std::cout << "Depart trouve : " << j << "  " << i;
            }
            else if (matrice[i][j] == Base) {
                posFinishX->push_back(j);
                posFinishY->push_back(i);
                *nbr_arrive = *nbr_arrive + 1;
                // std::cout << "Arrive trouve : " << j << "  " << i;
            }
        }
        //std::cout<< std::endl;
    }
}