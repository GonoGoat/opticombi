#pragma once

#ifndef PARSEUR_H
#define PARSEUR_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>
#include "data.h"

void parsage(std::string nom_fichier, std::vector<std::vector<int>>* matrice, int* nombreLignes, int* nombreColonnes);
Matrice conversion(const std::string& caractere);

#endif