#include <iostream>
#include <string>
#include <vector>
#include "data.h"

void getPositionsOfSequence (mapStruct* mapParams, svgStruct* svgParams, outputStruct* outputParams);
void Engine(mapStruct* mapParams, outputStruct* outputParams);
void Deplacement(char* dir, int* pos_x, int* pos_y);
void Verification_deplacement (mapStruct* mapParams, int* depl_x, int* depl_y, char* dir);
void Portail(std::vector<std::vector<int>>* matrice, std::vector<std::vector<int>>* matrice_mobile, int couleur, int* depl_x, int* depl_y, int* succes);
void Verification_Anti_Tank(mapStruct* mapParams, int* depl_x, int* depl_y);
void Verification_Anti_Tank_parcour_vertical(std::vector<std::vector<int>>* matrice, std::vector<std::vector<int>>* matrice_mobile, int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes);
void Verification_Anti_Tank_parcour_horizontal(std::vector<std::vector<int>>* matrice, std::vector<std::vector<int>>* matrice_mobile, int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes);
void Tir(std::vector<std::vector<int>>* matrice, std::vector<std::vector<int>>* matrice_mobile, int depl_x, int depl_y, char dir_laser, int* succes);
void glace(char* direction, std::vector<std::vector<int>>* matrice, std::vector<std::vector<int>>* matrice_mobile, int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes);
void glace_fine(char* direction, std::vector<std::vector<int>>* matrice, std::vector<std::vector<int>>* matrice_mobile, int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes);
void path(char* direction, std::vector<std::vector<int>>* matrice, std::vector<std::vector<int>>* matrice_mobile, int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes);