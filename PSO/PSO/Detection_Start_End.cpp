#include "Detection_Start_End.h"
/**
 * @brief D�tecte le point de d�part et les points d'arriv�e d'une carte
 *
 * @param matrice IN - Carte pars�e
 * @param nombreLignes IN - Nombres de ligne de la matrice
 * @param nombreColonnes IN - Nombres de colonnes de la matrice
 * @param posOriginX OUT - Coordonn�e X du point de d�part
 * @param posOriginY OUT - Coordonn�e Y du point de d�part
 * @param posFinishX OUT - Coordonn�es X des arriv�es d�tect�es
 * @param posFinishY OUT - Coordonn�es Y des arriv�es d�tect�es
 * @param nbr_arrive OUT - Nombre d'arriv�es d�tect�es
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