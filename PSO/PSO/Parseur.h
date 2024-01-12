#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>
#include "data.h"

void parsage(mapStruct* mapParams);
Matrice conversion(const std::string& caractere);
void position_valide(mapStruct* mapParams);
void Verification_Anti_Tank(mapStruct* mapParams, moveStruct* moveParams);