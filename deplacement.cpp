#include <iostream>
#include <string>

void Deplacement(int posOriginX, int posOriginY, int posFinishX, int posFinishY, std::string tankDir) {

	int left = 0;
	int right = 0;
	int up = 0;
	int down = 0;
	int disX = posFinishX - posOriginX;
	int disY = posFinishY - posOriginY;
	std::string Output = "";

	if (disX > 0) {
		right = disX;
		if (tankDir != "R") {
			Output += "R";
			tankDir = "R";
		}
		Output += std::string(right, 'R');
	}
	else {
		left = std::abs(disX);
		if (tankDir != "L") {
			Output += "L";
			tankDir = "L";
		}
		Output += std::string(left, 'L');
	}

	if (disY > 0) {
		up = disY;
		if (tankDir != "U") {
			Output += "U";
			tankDir = "U";
		}
		Output += std::string(up, 'U');
	}
	else {
		down = std::abs(disY);
		if (tankDir != "D") {
			Output += "D";
			tankDir = "D";
		}
		Output += std::string(down, 'D');
	}

	std::cout << Output << std::endl;

}