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
#include <vector>

std::string getRect(int x, int y, int width, int height, std::string style);
std::string getCircle(int cx, int cy, int r, std::string style);
std::string getLine (int x1, int y1, int x2, int y2, std::string style);
std::string getSectionWithElement(std::string* element);
void drawSVG(mapStruct* mapParams, svgStruct* svgParams);
