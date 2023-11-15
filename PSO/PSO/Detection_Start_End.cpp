#include "Detection_Start_End.h"


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
    Tunnel_Red, Tunnel_Green, Tunnel_Blue, Tunnel_Cyan, Tunnel_Yellow, Tunnel_Pink, Tunnel_White, Tunnel_Dark

};

void detection(int matrice[16][16], int* posOriginX, int* posOriginY, int* posFinishX, int* posFinishY, int* nbr_arrive) {

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
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