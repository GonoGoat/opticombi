#include "Detection_Start_End.h"
/**
 * @brief D�tecte le point de d�part et les points d'arriv�e d'une carte
 *
 * @param matrice Carte pars�e
 * @param nombreLignes Nombres de ligne de la matrice
 * @param nombreColonnes Nombres de colonnes de la matrice
 * @param posOriginX RES - Coordonn�e X du point de d�part
 * @param posOriginY RES - Coordonn�e Y du point de d�part
 * @param posFinishX RES - Coordonn�es X des arriv�es d�tect�es
 * @param posFinishY RES - Coordonn�es Y des arriv�es D�tect�es
 * @param nbr_arrive RES - Nombre d'arriv�es d�tect�es
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