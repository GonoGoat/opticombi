#pragma once

// "Main"
// Dessin matrice (contour + points des cases)
// Dessin séquence
// Dessin ligne entre 2 points
// Dessin trajectoire
#include "Parseur.h"
#include "Detection_Start_End.h"
#include "Logique_jeu.h"
#include <string>

std::string getRect(int x, int y, int width, int height, std::string style);
std::string getCircle(int cx, int cy, int r, std::string style);
std::string getSectionWithElement(std::string element);
void drawSVG(std::vector<std::vector<int>> matrice, int Origine_x, int Origine_y, std::vector<int> Finish_x, std::vector<int> Finish_y, int nbr_arrive, std::string sequence,std::string output_file, int nombreLignes, int nombreColonnes);
