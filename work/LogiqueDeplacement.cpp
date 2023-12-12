#include "LogiqueDeplacement.h"

/**
 * @brief Génération d'une séquence pour arriver d'un point à un autre (plus court chemin)
 * 
 * @param posOriginX IN - Coordonnée X de la position de départ
 * @param posOriginY IN - Coordonnée Y de la position de départ
 * @param posFinishX IN - Coordonnées X des positions d'arrivée
 * @param posFinishY IN - Coordonnées Y des positions d'arrivée
 * @param tankDir IN - Direction du tank au début de la séquence
 * @param Output OUT - Séquence générée
 * @param selection IN - Arrivée choisie
 */
void Deplacement(int *posOriginX, int *posOriginY, int *posFinishX, int *posFinishY, char *tankDir, std::string* Output, int* selection) {

	int left = 0;
	int right = 0;
	int up = 0;
	int down = 0;
	int disX = posFinishX[*selection] - *posOriginX;
	int disY = posFinishY[*selection] - *posOriginY;
	*Output = "";

	if (disX > 0) {
		right = disX;
		if (*tankDir != 'R') {
			*Output += 'R';
			*tankDir = 'R';
		}
		*Output += std::string(right, 'R');
	}
	else {
		left = std::abs(disX);
		if (*tankDir != 'L') {
			*Output += 'L';
			*tankDir = 'L';
		}
		*Output += std::string(left, 'L');
	}

	if (disY > 0) {
		up = disY;
		if (*tankDir != 'U') {
			*Output += 'U';
			*tankDir = 'U';
		}
		*Output += std::string(up, 'U');
	}
	else {
		down = std::abs(disY);
		if (*tankDir != 'D') {
			*Output += 'D';
			*tankDir = 'D';
		}
		*Output += std::string(down, 'D');
	}

	//std::cout << *Output << std::endl;

}