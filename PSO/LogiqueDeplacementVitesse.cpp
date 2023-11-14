#include "LogiqueDeplacementVitesse.h"

void DeplacementVitesse(float* vitesse_x, float* vitesse_y, char* tankDir, std::string* Output) {

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
		if (*tankDir != 'R') {
			*Output += 'R';
			*tankDir = 'R';
		}
		*Output += 'R';
	}
	else if(*vitesse_x < 0) {
		if (*tankDir != 'L') {
			*Output += 'L';
			*tankDir = 'L';
		}
		*Output += 'L';
	}

	if (*vitesse_y > 0) {
		if (*tankDir != 'D') {
			*Output += 'D';
			*tankDir = 'D';
		}
		*Output += 'D';
	}
	else if (*vitesse_y < 0) {
		if (*tankDir != 'U') {
			*Output += 'U';
			*tankDir = 'U';
		}
		*Output += 'U';
	}
}