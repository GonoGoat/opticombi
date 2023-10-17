#include "Detection_Start_End.h"

void detection(std::string matrice[16][16], int* posOriginX, int* posOriginY, int* posFinishX, int* posFinishY) {

    bool depart = false;
    bool fin = false;

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            //std::cout << j;
            if (matrice[i][j] == " Tu"|| matrice[i][j] == "Tu"|| matrice[i][j] == " Tu ") {
                *posOriginX = j;
                *posOriginY = i;
                depart = true;
                //std::cout << "Depart trouve : " << j << "  " << i;
            }
            else if(matrice[i][j] == "b"||matrice[i][j] == " b" || matrice[i][j] == " b ") {
                *posFinishX = j;
                *posFinishY = i;
                fin = true;
                //std::cout << "Arrive trouve : " << j << "  " << i;
            }
            else if (depart && fin) {
                return;
            }
        }
       //std::cout<< std::endl;
    }
}