#include <iostream>
#include <string>

void Engine(int matrice[16][16], const std::string& sequence, int* posX, int* posY, char *dir_previous, bool *succes);
void Deplacement(char* dir, int* pos_x, int* pos_y);
void Verification_deplacement (int matrice[16][16], int* depl_x, int* depl_y, int* pos_x, int* pos_y, bool *succes);
void Portail(int matrice[16][16], int couleur,int* pos_x, int* pos_y, int* depl_x, int* depl_y);
void Tir();