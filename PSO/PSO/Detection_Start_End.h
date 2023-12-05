#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "data.h"

void detection(std::vector<std::vector<int>>* matrice, int* nombreLignes, int* nombreColonnes, int* posOriginX, int* posOriginY, std::vector<int>* posFinishX, std::vector<int>* posFinishY, int* nbr_arrive);