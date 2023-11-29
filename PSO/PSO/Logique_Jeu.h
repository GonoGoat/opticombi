#include <iostream>
#include <string>

void Engine(int matrice[16][16], const std::string& sequence, int* posX, int* posY, char *dir_previous, int *succes);
void Deplacement(char* dir, int* pos_x, int* pos_y);
void Verification_deplacement (int matrice[16][16], int* depl_x, int* depl_y, int* pos_x, int* pos_y, int *succes);
void Portail(int matrice[16][16], int couleur, int* depl_x, int* depl_y, int* succes);
void Verification_Anti_Tank(int matrice[16][16], int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes);
void Verification_Anti_Tank_parcour_vertical(int matrice[16][16], int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes);
void Verification_Anti_Tank_parcour_horizontal(int matrice[16][16], int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes);
void Tir(int matrice[16][16], int depl_x, int depl_y, char dir_laser, int* succes);