#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>
#include "data.h"

struct parsageParam {
	std::string* nom_fichier;
	//int*** matrice;
	std::vector<std::vector<int>>* matrice_fixe;
	std::vector<std::vector<int>>* matrice_mobile;
	int* nombreLignes;
	int* nombreColonnes;
};

void parsage(parsageParam* params);
Matrice conversion(const std::string& caractere);