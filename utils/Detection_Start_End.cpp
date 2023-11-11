#include "../Libs/Detection_Start_End.h"

/**
 * @brief Détecte un départ et une arrivée d'une carte
 * 
 * @param matrice Carte parsée
 * @param posOriginX RES - Coordonnée X du point de départ
 * @param posOriginY RES - Coordonnée Y du point de départ
 * @param posFinishX RES - Coordonnées X des arrivées détectées
 * @param posFinishY RES - Coordonnées Y des arrivées Détectées
 * @param nbr_arrive RES - Nombre d'arrivées détectées
 */
void detection(std::string matrice[16][16], int* posOriginX, int* posOriginY, int* posFinishX, int* posFinishY, int* nbr_arrive) {

    for (int i = 0; i < 16; i++) { // Pour chaque ligne
        for (int j = 0; j < 16; j++) { // Pour chaque colonne
            //std::cout << j;
            if (matrice[i][j] == " Tu"|| matrice[i][j] == "Tu"|| matrice[i][j] == " Tu ") {
                *posOriginX = j;
                *posOriginY = i;
                //std::cout << "Depart trouve : " << j << "  " << i;
            }
            else if(matrice[i][j] == "b"||matrice[i][j] == " b" || matrice[i][j] == " b ") {
                posFinishX[*nbr_arrive] = j;
                posFinishY[*nbr_arrive] = i;
                *nbr_arrive = *nbr_arrive + 1;
                //std::cout << "Arrive trouve : " << j << "  " << i;
            }
        }
       //std::cout<< std::endl;
    }
}