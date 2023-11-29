#ifndef LJ_H
#define LJ_H

#include <iostream>
#include <string>
#include <vector>
#include "data.h"

void getPositionsOfSequence (std::vector<std::vector<int>> matrice, std::string& sequence, std::vector<int>* trajX, std::vector<int>* trajY, int* succes);
void Engine(std::vector<std::vector<int>> matrice, const std::string& sequence, int* posX, int* posY, char *dir_previous, int *succes);
void Deplacement(char* dir, int* pos_x, int* pos_y);
void Verification_deplacement (std::vector<std::vector<int>> matrice, int* depl_x, int* depl_y, int* pos_x, int* pos_y, int *succes);
void Portail(std::vector<std::vector<int>> matrice, int couleur, int* depl_x, int* depl_y);
void Verification_Anti_Tank(std::vector<std::vector<int>> matrice, int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes);
void Tir();

#endif