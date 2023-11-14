#include "Algo_PSO.h"

std::string Algo_PSO(int* nbre_thread, int* nbr_iteration_max, int* Origine_x, int* Origine_y, int* Finish_x, int* Finish_y) {
	int* posX = new int[*nbre_thread]; int* posY = new int[*nbre_thread];
	int* posX_final = new int[*nbre_thread];int* posY_final = new int[*nbre_thread];
	float* vitX = new float[*nbre_thread];float* vitY = new float[*nbre_thread];
	int* p_bestX = new int[*nbre_thread];int* p_bestY = new int[*nbre_thread];
	int* score_p_best = new int[*nbre_thread];
	int g_bestX, g_bestY;
	int score_g_best = 0;
	int n1 = 150;int n2 = 2;
	float omega = 1, c1 = 1, c2 = 1, random_1, random_2;
	float* distance_finish = new float[*nbre_thread];
	int* score = new int[*nbre_thread];
	char* directionTank = new char[*nbre_thread];
	std::string* Output = new std::string[*nbre_thread];
	int nbr_iteration_t = 0;

	for (int i = 0; i < *nbre_thread; i++) {
		if (i == 0) {
			posX[i] = *Origine_x;
			posY[i] = *Origine_y;
		}
		else {
			posX[i] = rand() % 16;
			posY[i] = rand() % 16;
		}
		vitX[i] = rand() % 16 - 8;
		vitY[i] = rand() % 16 - 8;
		score_p_best[i] = -9999;
		directionTank[i] = 'U';
		DeplacementVitesse(&vitX[i], &vitY[i], &directionTank[i], &Output[i]);
		posX_final[i] = posX[i] + vitX[i];
		posY_final[i] = posY[i] + vitY[i];
		//std::cout << "Px = " << posX[i] << "Py = " << posY[i] << "Vx = " << vitX[i] << "Vy = " << vitY[i] << "Scorep = " << score_p_best[i] << "Dir = " << directionTank[i] << "PosFinX = " << posX_final[i] << "PosFinY = " << posY_final[i] << "Output = " << Output[i] << std::endl;
	}

	while (nbr_iteration_t < *nbr_iteration_max) {
		for (int i = 0; i < *nbre_thread; i++) {
			distance_finish[i] = sqrt((Finish_x[0] - posX_final[i]) * (Finish_x[0] - posX_final[i]) + (Finish_y[0] - posY_final[i]) * (Finish_y[0] - posY_final[i]));
			//std::cout << "dist = " << distance_finish[i] << "finX = " << Finish_x[0] << "finY = " << Finish_y[0] << "posXfin = " << posX_final[i] << "posYfin = " << posY_final[i] << std::endl;
			if (int(distance_finish[0]) == 0) {
				std::cout << "OK1" << std::endl;
				break;
			}
			score[i] = n1 / distance_finish[i] - n2 * std::size(Output[i]);
			//std::cout << "score = " << score[i] << "n1 = " << n1 << "dist = " << distance_finish[i] << "n2 = " << n2 << "sizeOutput = " << std::size(Output[i]) << std::endl;
			if (score[i] > score_p_best[i]) {
				p_bestX[i] = posX_final[i];
				p_bestY[i] = posY_final[i];
				score_p_best[i] = score[i];
			}
			if (score[i] > score_g_best) {
				g_bestX = posX_final[i];
				g_bestY = posY_final[i];
				score_g_best = score[i];
			}
		}

		//std::cout << int(distance_finish[0]) << std::endl;
		if (int(distance_finish[0]) == 0) break;

		random_1 = float(rand() % 100) / 100;
		random_2 = float(rand() % 100) / 100;
		//std::cout << random_1 << random_2 << std::endl;

		for (int i = 0; i < *nbre_thread; i++) {
			vitX[i] = omega * vitX[i] + c1 * random_1 * (p_bestX[i] - posX_final[i]) + c2 * random_2 * (g_bestX - posX_final[i]);
			vitY[i] = omega * vitY[i] + c1 * random_1 * (p_bestY[i] - posY_final[i]) + c2 * random_2 * (g_bestY - posY_final[i]);
			std::cout << vitX[i] << std::endl;
			DeplacementVitesse(&vitX[i], &vitY[i], &directionTank[i], &Output[i]);
			posX_final[i] = posX_final[i] + vitX[i];
			posY_final[i] = posY_final[i] + vitY[i];
			//std::cout << "vitX = " << vitX[i] << "omega = " << omega << "c1 = " << c1 << "random1 = " << random_1 << "pBestX = " << p_bestX[i] << "posXfin = " << posX_final[i] << "c2 = " << c2 << "random2 = " << random_2 << "g_bestX" << g_bestX << std::endl;
		}

		nbr_iteration_t++;
	}

	delete[] posX;
	delete[] posY;
	delete[] posX_final;
	delete[] posY_final;
	delete[] vitX;
	delete[] vitY;
	delete[] p_bestX;
	delete[] p_bestY;
	delete[] score_p_best;
	delete[] distance_finish;
	delete[] score;
	delete[] directionTank;

	return Output[0];
	delete[] Output;
}