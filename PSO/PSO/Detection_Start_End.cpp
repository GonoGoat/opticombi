#include "Detection_Start_End.h"
/**
 * @brief Détecte le point de départ et les points d'arrivée d'une carte
 *
 * @param matrice IN - Carte parsée
 * @param nombreLignes IN - Nombres de ligne de la matrice
 * @param nombreColonnes IN - Nombres de colonnes de la matrice
 * @param posOriginX OUT - Coordonnée X du point de départ
 * @param posOriginY OUT - Coordonnée Y du point de départ
 * @param posFinishX OUT - Coordonnées X des arrivées détectées
 * @param posFinishY OUT - Coordonnées Y des arrivées détectées
 * @param nbr_arrive OUT - Nombre d'arrivées détectées
 */
void detection(std::vector<std::vector<int>>* matrice, int* nombreLignes, int* nombreColonnes, int* posOriginX, int* posOriginY, std::vector<int>* posFinishX, std::vector<int>* posFinishY, int* nbr_arrive) {

    for (int i = 0; i < *nombreLignes; i++) { // Pour chaque ligne
        for (int j = 0; j < *nombreColonnes; j++) { // Pour chaque colonne
            //std::cout << j;
            if ((*matrice)[i][j] == Tank) {
                *posOriginX = j;
                *posOriginY = i;
                // std::cout << "Depart trouve : " << j << "  " << i;
            }
            else if ((*matrice)[i][j] == Base) {
                posFinishX->push_back(j);
                posFinishY->push_back(i);
                *nbr_arrive = *nbr_arrive + 1;
                // std::cout << "Arrive trouve : " << j << "  " << i;
            }
        }
        //std::cout<< std::endl;
    }
}