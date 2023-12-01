#include "Algo_PSO.h"

std::string Algo_PSO(std::vector<std::vector<int>>* matrice, int* nbre_thread, int* nbr_iteration_max, int* Origine_x, int* Origine_y, std::vector<int>* Finish_x, std::vector<int>* Finish_y, int* nbr_particule) {
	std::vector<int> posX(*nbre_thread); std::vector<int> posY(*nbre_thread);
	std::vector<int> posX_final(*nbre_thread); std::vector<int> posY_final(*nbre_thread);
	std::vector<float> vitX(*nbre_thread); std::vector<float> vitY(*nbre_thread);
	std::vector<int> p_bestX(*nbre_thread); std::vector<int> p_bestY(*nbre_thread);
	std::vector<int> score_p_best(*nbre_thread);
	std::vector<int> g_bestX(*nbre_thread / *nbr_particule); std::vector<int> g_bestY(*nbre_thread / *nbr_particule);
	std::vector<int> score_g_best(*nbre_thread / *nbr_particule);
	int n1 = 150;int n2 = 2;
	float omega = 1, c1 = 1, c2 = 1, random_1, random_2;
	std::vector<float> distance_finish(*nbre_thread);
	std::vector<int> score(*nbre_thread);
	std::vector<char> directionTank(*nbre_thread);
	std::vector<char> directionOriginalTank(*nbre_thread);
	std::vector<std::string> Output(*nbre_thread);
	int nbr_iteration_t = 0;
	int n;
	std::vector<bool> become_finish(*nbre_thread);
	bool found_finish = false;
	std::vector<std::string> finish_Output(*nbre_thread / *nbr_particule);
	int succes;

	n = *nbre_thread / *nbr_particule;
	for (int i = 0; i < n; i++) {
		score_g_best[i] = -9999;
		finish_Output[i] = "";
	}

	n = 0;
	for (int i = 0; i < *nbre_thread; i++) {
		if (i == n * *nbr_particule) {
			posX[i] = *Origine_x;
			posY[i] = *Origine_y;
			directionTank[i] = 'U';
			n++;
		}
		else {
			posX[i] = rand() % 16;
			posY[i] = rand() % 16;
			directionTank[i] = '/';
		}
		vitX[i] = rand() % 16 - 8;
		vitY[i] = rand() % 16 - 8;
		score_p_best[i] = -9999;
		posX_final[i] = posX[i];
		posY_final[i] = posY[i];
		DeplacementVitesse(&vitX[i], &vitY[i], &directionTank[i], &Output[i]);
		directionOriginalTank[i] = directionTank[i];
		Engine(matrice, Output[i], &posX_final[i], &posY_final[i], &directionTank[i], &succes);
		become_finish[i] = false;
		//std::cout << "Px = " << posX[i] << "Py = " << posY[i] << "Vx = " << vitX[i] << "Vy = " << vitY[i] << "Scorep = " << score_p_best[i] << "Dir = " << directionTank[i] << "PosFinX = " << posX_final[i] << "PosFinY = " << posY_final[i] << "Output = " << Output[i] << std::endl;
	}

	while (nbr_iteration_t < *nbr_iteration_max) {
		n = 0;
		for (int i = 0; i < *nbre_thread; i++) {
			if (i == (n+1) * *nbr_particule) {
				n++;
			}
			distance_finish[i] = sqrt((Finish_x[0][n] - posX_final[i]) * (Finish_x[0][n] - posX_final[i]) + (Finish_y[0][n] - posY_final[i]) * (Finish_y[0][n] - posY_final[i]));
			//std::cout << "dist = " << distance_finish[i] << "finX = " << Finish_x[0] << "finY = " << Finish_y[0] << "posXfin = " << posX_final[i] << "posYfin = " << posY_final[i] << std::endl;
			if (int(distance_finish[i]) == 0 && become_finish[i] == false) {
				/*std::cout << "Nouveau chemin vers finish trouve !" << std::endl;
				std::cout << "PositionX = " << posX[i] << " PositionY = " << posY[i] << std::endl;
				std::cout << "Particule " << i << " + " << "Finish " << n << std::endl;*/

				*nbre_thread = *nbre_thread + *nbr_particule;
				Finish_x->push_back(posX[i]);
				Finish_y->push_back(posY[i]);
				become_finish[i] = true;

				/*std::cout << "= Finish " << finish_Output.size() << std::endl;
				std::cout << Output[i] << " + " << finish_Output[n] << std::endl;
				std::cout << std::endl;*/

				if (Output[i].back() != finish_Output[n][0]) Output[i] += finish_Output[n][0];
				finish_Output.push_back(Output[i] + finish_Output[n]);

				g_bestX.push_back(0);
				g_bestY.push_back(0);
				score_g_best.push_back(-9999);
				for (int i = 0; i < *nbr_particule; i++) {
					posX.push_back(rand() % 16);
					posY.push_back(rand() % 16);
					vitX.push_back(rand() % 16 - 8);
					vitY.push_back(rand() % 16 - 8);
					p_bestX.push_back(NULL);
					p_bestY.push_back(NULL);
					score_p_best.push_back(-9999);
					distance_finish.push_back(NULL);
					score.push_back(NULL);
					directionTank.push_back('/');
					Output.push_back("");
					become_finish.push_back(false);
				}
				directionTank[*nbre_thread - *nbr_particule] = 'U';
				posX[*nbre_thread - *nbr_particule] = *Origine_x;
				posY[*nbre_thread - *nbr_particule] = *Origine_y;
				for (int j = *nbr_particule; j > 0; j--) {
					posX_final.push_back(posX[*nbre_thread - j]);
					posY_final.push_back(posY[*nbre_thread - j]);
					DeplacementVitesse(&vitX[*nbre_thread - j], &vitY[*nbre_thread - j], &directionTank[*nbre_thread - j], &Output[*nbre_thread - j]);
					directionOriginalTank.push_back(directionTank[*nbre_thread - j]);
					Engine(matrice, Output[*nbre_thread - j], &posX_final[*nbre_thread - j], &posY_final[*nbre_thread - j], &directionTank[*nbre_thread - j], &succes);
				}
			}
			if (int(distance_finish[n * *nbr_particule]) == 0) {
				std::cout << "Chemin trouve!" << std::endl;
				found_finish = true;
				break;
			}
			if (become_finish[i] == false) {
				score[i] = n1 / distance_finish[i] - n2 * std::size(Output[i]);
				//std::cout << "score = " << score[i] << "n1 = " << n1 << "dist = " << distance_finish[i] << "n2 = " << n2 << "sizeOutput = " << std::size(Output[i]) << std::endl;
				if (score[i] > score_p_best[i]) {
					p_bestX[i] = posX_final[i];
					p_bestY[i] = posY_final[i];
					score_p_best[i] = score[i];
				}
				if (score[i] > score_g_best[n]) {
					g_bestX[n] = posX_final[i];
					g_bestY[n] = posY_final[i];
					score_g_best[n] = score[i];
				}
			}
		}

		if (int(distance_finish[n * *nbr_particule]) == 0) break;

		random_1 = float(rand() % 100) / 100;
		random_2 = float(rand() % 100) / 100;
		//std::cout << random_1 << random_2 << std::endl;

		n = 0;
		for (int i = 0; i < *nbre_thread; i++) {
			if (i == (n + 1) * *nbr_particule) {
				n++;
			}
			if (become_finish[i] == false) {
				vitX[i] = omega * vitX[i] + c1 * random_1 * (p_bestX[i] - posX_final[i]) + c2 * random_2 * (g_bestX[n] - posX_final[i]);
				vitY[i] = omega * vitY[i] + c1 * random_1 * (p_bestY[i] - posY_final[i]) + c2 * random_2 * (g_bestY[n] - posY_final[i]);
				//std::cout << vitX[i] << std::endl;
				DeplacementVitesse(&vitX[i], &vitY[i], &directionTank[i], &Output[i]);
				directionTank[i] = directionOriginalTank[i];
				posX_final[i] = posX[i];
				posY_final[i] = posY[i];
				Engine(matrice, Output[i], &posX_final[i], &posY_final[i], &directionTank[i], &succes);
				//std::cout << "vitX = " << vitX[i] << "omega = " << omega << "c1 = " << c1 << "random1 = " << random_1 << "pBestX = " << p_bestX[i] << "posXfin = " << posX_final[i] << "c2 = " << c2 << "random2 = " << random_2 << "g_bestX" << g_bestX << std::endl;
			}
		}

		std::cout << "Iteration " << nbr_iteration_t << std::endl;
		std::cout << "Nombre de particules : " << *nbre_thread << std::endl;
		nbr_iteration_t++;
		omega = omega - 0.03;
		if (omega < 0) omega = 0;
	}

	if (found_finish == true) {
		return finish_Output.back();
	}
	else {
		return "Rien trouve :(";
	}
}