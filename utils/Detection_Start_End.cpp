#include "../Libs/Detection_Start_End.h"

void detection(std::string matrice[16][16], int* posOriginX, int* posOriginY, int* posFinishX, int* posFinishY, int* nbr_arrive) {

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
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