#include "Logique_Jeu.h"

enum Matrice
{
    Dirt, //0
    Tank, //1
    Base, //2
    Water, //3
    Sollid_Block, //4
    Movable_Block, //5
    Bricks, //6
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

//posOriginX et posOriginY position tank au début de la séquence.

void Engine(int matrice[16][16], const std::string& sequence, int* posX, int* posY, char *dir_previous, bool* succes) {
    int deplacement_x, deplacement_y;

    for (char dir_actuelle : sequence)
    {
        if (dir_actuelle == *dir_previous) {

            deplacement_x = *posX;
            deplacement_y = *posY;

            Deplacement(&dir_actuelle, &deplacement_x, &deplacement_y);
            Verification_deplacement(matrice, &deplacement_x, &deplacement_y, posX, posY, succes);
            std::cout << "deplacement x : " << *posX << " |deplacement y : " << *posY << std::endl;
        }
        else if (dir_actuelle == 'F') {
            Tir();
        }
        else {
            *dir_previous = dir_actuelle;
        }
    }
}

void Deplacement(char* dir, int* pos_x, int* pos_y) {
    switch (*dir) {
    case 'U':
        *pos_y -= 1;
        break;
    case 'D':
        *pos_y += 1;
        break;
    case 'R':
        *pos_x += 1;
        break;
    case 'L':
        *pos_x -= 1;
        break;
    }
    //std::cout << "Calcul x : " << *pos_x << " |calcul y : " << *pos_y << std::endl;
}


void Verification_deplacement(int matrice[16][16], int* depl_x, int* depl_y, int* pos_x, int* pos_y, bool* succes){

    if (*depl_x > 15) {
        *depl_x = 15;
    }
    else if (*depl_x < 0) {
        *depl_x = 0;
    }
    else if (*depl_y > 15) {
        *depl_y = 15;
    }
    else if (*depl_y < 0) {
        *depl_y = 0;
    }
    else {
        std::cout << *depl_x << " | " << *depl_y << "  ";
        std::cout << matrice[*depl_x][*depl_y];
        switch (matrice[*depl_y][*depl_x])
        {
        case Dirt :
        case Bridge:
            *pos_x = *depl_x;
            *pos_y = *depl_y;
            std::cout << "  Dirt ";
            break;
        case Base :
            *pos_x = *depl_x;
            *pos_y = *depl_y;
            *succes = true;
            std::cout << "  Base ";
            break;
        case Water :
            break;
        case Way_U :
        case Way_D :
        case Way_R :
        case Way_L :
            break;
        case Ice :
            break;
        case Thin_Ice :
            break;
        case Tunnel_Red :
        case Tunnel_Green :
        case Tunnel_Blue :
        case Tunnel_Cyan : 
        case Tunnel_Yellow :
        case Tunnel_Pink :
        case Tunnel_White :
        case Tunnel_Dark :
            Portail(matrice, matrice[*depl_y][*depl_x], depl_x, depl_y, pos_x, pos_y);
            break;
        default:
            break;
        }
    }

}

void Portail(int matrice[16][16], int couleur, int* pos_x, int* pos_y, int* depl_x, int* depl_y)
{
    bool sortie = false;

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            if (matrice[i][j] == couleur && (i != *pos_y || j != *pos_x)) {
                *depl_y = i;
                *depl_x = j;
                sortie = true;
                break;
            }
        }
        if (sortie) {
            break;
        }
        std::cout << i << std::endl;
    }
}

void Tir()
{
}

