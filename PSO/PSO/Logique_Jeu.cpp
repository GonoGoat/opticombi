#include "Logique_Jeu.h"

void Analyse(std::string matrice[16][16], int* posOriginX, int* posOriginY, const std::string& sequence, int* posX, int* posY) {

	char precedent;
	char deplacement[2];
	bool passer = false;

	for(char i : sequence)
	{
		precedent = i;
		if (passer == false) {
			deplacement[0] = i;
			passer = true;
		}
		else if (passer == true) {
			deplacement[1] = i;

		}

	}
}