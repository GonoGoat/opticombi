#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>
#include "data.h"

void parsage(std::string nom_fichier, int matrice[16][16]);
Matrice conversion(const std::string& caractere);