#include "../Libs/Detection_Start_End.h"

<<<<<<< HEAD:utils/Detection_Start_End.cpp
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
=======

enum Matrice
{
    Dirt,
    Tank,
    Base,
    Water,
    Sollid_Block,
    Movable_Block,
    Bricks,
    Anti_Tank_U, Anti_Tank_D, Anti_Tank_R, Anti_Tank_L,
    Mirror_UR, Mirror_UL, Mirror_DR, Mirro_DL,
    Way_U, Way_D, Way_R, Way_L,
    Crystal_Block,
    Rotative_Mirror_UR, Rotative_Mirror_UL, Rotative_Mirror_DR, Rotative_Mirror_DL,
    Ice,
    Thin_Ice,
    Tunnel_Red, Tunnel_Green, Tunnel_Blue, Tunnel_Cyan, Tunnel_Yellow, Tunnel_Pink, Tunnel_White, Tunnel_Dark,
    Bridge

};

void detection(int matrice[16][16], int* posOriginX, int* posOriginY, int* posFinishX, int* posFinishY, int* nbr_arrive) {
>>>>>>> origin/Thomas:PSO/PSO/Detection_Start_End.cpp

    for (int i = 0; i < 16; i++) { // Pour chaque ligne
        for (int j = 0; j < 16; j++) { // Pour chaque colonne
            //std::cout << j;
            if (matrice[i][j] == Tank) {
                *posOriginX = j;
                *posOriginY = i;
                //std::cout << "Depart trouve : " << j << "  " << i;
            }
            else if(matrice[i][j] == Base) {
                posFinishX[*nbr_arrive] = j;
                posFinishY[*nbr_arrive] = i;
               *nbr_arrive = *nbr_arrive + 1;
               //std::cout << "Arrive trouve : " << j << "  " << i;
            }
        }
       //std::cout<< std::endl;
    }
}