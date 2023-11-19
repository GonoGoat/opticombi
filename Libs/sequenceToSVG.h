// "Main"
// Dessin matrice (contour + points des cases)
// Dessin séquence
// Dessin ligne entre 2 points
// Dessin trajectoire
#include "..\utils\Parseur.cpp"
#include "..\utils\Detection_Start_End.cpp"
#include <string>

std::string getInitSVG ();
std::string getRect();
std::string getCircle();
std::string getStart();
std::string getEnd();
std::string getSection();
void drawSVG(std::string output_file);