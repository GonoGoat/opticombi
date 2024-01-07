#include "LogiqueDeplacementVitesse.h"

void DeplacementVitesse(float* vitesse_x, float* vitesse_y, char* tankDir, std::string* Output, int* nbr_modif) {

	if (abs(*vitesse_x) > abs(*vitesse_y)) {
		*vitesse_y = 0;
		if (*vitesse_x >= 0) *vitesse_x = 1;
		else *vitesse_x = -1;
	}
	else {
		*vitesse_x = 0;
		if (*vitesse_y >= 0) *vitesse_y = 1;
		else *vitesse_y = -1;
	}

	if (*vitesse_x > 0) {
		if (*tankDir != 'R' && *tankDir != '/') {
			*Output += 'R';
			(*nbr_modif)++;
			//*tankDir = 'R';
		}
		else if (*tankDir = '/') *tankDir = 'R';
		*Output += 'R';
		(*nbr_modif)++;
	}
	else if (*vitesse_x < 0) {
		if (*tankDir != 'L' && *tankDir != '/') {
			*Output += 'L';
			(*nbr_modif)++;
			//*tankDir = 'L';
		}
		else if (*tankDir = '/') *tankDir = 'L';
		*Output += 'L';
		(*nbr_modif)++;
	}

	if (*vitesse_y > 0) {
		if (*tankDir != 'D' && *tankDir != '/') {
			*Output += 'D';
			(*nbr_modif)++;
			//*tankDir = 'D';
		}
		else if (*tankDir = '/') *tankDir = 'D';
		*Output += 'D';
		(*nbr_modif)++;
	}
	else if (*vitesse_y < 0) {
		if (*tankDir != 'U' && *tankDir != '/') {
			*Output += 'U';
			(*nbr_modif)++;
			//*tankDir = 'U';
		}
		else if (*tankDir = '/') *tankDir = 'U';
		*Output += 'U';
		(*nbr_modif)++;
	}
}