#include "Algo_PSO.h"

std::string Algo_PSO(mapStruct* mapParams, psoStruct* psoParams) {
	std::vector<int> g_bestX(mapParams->nbr_arrive); std::vector<int> g_bestY(mapParams->nbr_arrive);
	std::vector<int> score_g_best(mapParams->nbr_arrive);
	int n1 = 150;int n2 = 2;
	float omega = 1, c1 = 1, c2 = 1, random_1, random_2;
	int nbr_iteration_t = 0;
	// Identifiant d'arrivée
	int n = 0;
	bool found_finish = false;
	std::vector<std::string> finish_Output(mapParams->nbr_arrive);

	//Threads pour la parral�lisation
	std::thread* thread;
	std::vector<std::thread*> threads;


	// Initialisation des arrivées
	for (int i = 0; i < mapParams->nbr_arrive; i++) {
		score_g_best[i] = -9999;
		finish_Output[i] = "";
	}

	//----Première ittération PSO/Initialisation-----
	std::vector<particleStruct> particles;

	// Particule originale
	particleStruct part;
	part.Direction_tank = '/';
	part.matrice_mobile = mapParams->matrice_mobile;
	part.success = En_vie;
	part.score_p_best = -9999;
	part.become_finish = false;
	part.taille_sequence = 0;
	// Adaptation de chaque particule
	for (int i = 0; i<psoParams->nbr_thread;i++) {
		particles.push_back(part);

		//Détection d'arrivée
		if (i == n * psoParams->nbr_particule) n++;

		// Adaptation des départs à la base
		if (i % psoParams->nbr_particule < psoParams->nbr_base) {
			particles[i].Origine_x = mapParams->Origine_x;
			particles[i].Origine_y = mapParams->Origine_y;
			particles[i].Direction_tank = mapParams->Direction_tank;
		}
		else {
			particles[i].Origine_x = rand() % 16;
			particles[i].Origine_y = rand() % 16;
		}
		particles[i].vitX = rand() % 16 - 8;
		particles[i].vitY = rand() % 16 - 8;
		particles[i].posX = particles[i].Origine_x;
		particles[i].posY = particles[i].Origine_y;
		if (mapParams->Finish_x.size() != 0 && mapParams->Finish_y.size() != 0) {
			particles[i].Finish_x = mapParams->Finish_x[n-1];
			particles[i].Finish_y = mapParams->Finish_y[n-1];
		}
		

		DeplacementVitesse(&particles[i].vitX, &particles[i].vitY, &particles[i].Direction_tank, &particles[i].Output);

		// Backup direction originale
		particles[i].Direction_original_tank = particles[i].Direction_tank;
		Engine(mapParams, &particles[i]);
		//std::cout << "Px = " << posX[i] << "Py = " << posY[i] << "Vx = " << vitX[i] << "Vy = " << vitY[i] << "Scorep = " << score_p_best[i] << "Dir = " << directionTank[i] << "PosFinX = " << posX_final[i] << "PosFinY = " << posY_final[i] << "Output = " << Output[i] << std::endl;
	}

	// Ittération PSO
	while (nbr_iteration_t < psoParams->nbr_iteration_max) {
		n = 0; // Reset du compteur d'arrivées
		for (int i = 0; i < psoParams->nbr_thread; i++) {
			// Changement de finish
			if (i == (n+1) * psoParams->nbr_particule) {
				n++;
			}
			particles[i].distance_finish = sqrt((particles[i].Finish_x - particles[i].posX) * (particles[i].Finish_x - particles[i].posX) + (particles[i].Finish_y - particles[i].posY) * (particles[i].Finish_y - particles[i].posY));
			//std::cout << "dist = " << distance_finish[i] << "finX = " << Finish_x[0] << "finY = " << Finish_y[0] << "posXfin = " << posX_final[i] << "posYfin = " << posY_final[i] << std::endl;

			// Passage d'une particule comme arrivée intermédiaire
			if (int(particles[i].distance_finish) == 0 && particles[i].become_finish == false) {
				/*std::cout << "Nouveau chemin vers finish trouve !" << std::endl;
				std::cout << "PositionX = " << particles[i].Origine_x << " PositionY = " << particles[i].Origine_y << std::endl;
				std::cout << "Particule " << i << " + " << "Finish " << n << std::endl;*/
				// Actualisation des arrivées possibles
				particles[i].become_finish = true;

				mapParams->nbr_arrive += 1;
				psoParams->nbr_thread += psoParams->nbr_particule;
				mapParams->Finish_x.push_back(particles[i].Origine_x);
				mapParams->Finish_y.push_back(particles[i].Origine_y);
				g_bestX.push_back(0);
				g_bestY.push_back(0);
				score_g_best.push_back(-9999);

				/*std::cout << "= Finish " << finish_Output.size() << std::endl;
				std::cout << Output[i] << " + " << finish_Output[n] << std::endl;
				std::cout << std::endl;*/

				// Si le trajet ajouté se finit avec position différente que celle de départ du trajet initial
				if (particles[i].Output.back() != finish_Output[n][0]) particles[i].Output += finish_Output[n][0];
				finish_Output.push_back(particles[i].Output + finish_Output[n]);

				// Regénération de particule pour nouvelle arrivée
				particleStruct newPart;
				newPart.p_bestX = NULL;
				newPart.p_bestY = NULL;
				newPart.score_p_best = -9999;
				newPart.distance_finish = NULL;
				newPart.score = NULL;
				newPart.Direction_tank = '/';
				newPart.Output = "";
				newPart.become_finish = false;
				newPart.matrice_mobile = mapParams->matrice_mobile;
				newPart.taille_sequence = 0;
				newPart.success = En_vie;
				newPart.Finish_x = mapParams->Finish_x.back();
				newPart.Finish_y = mapParams->Finish_y.back();

				// Adaptation de chaque particule
				for (int k = 0; k < psoParams->nbr_particule; k++) {
					particles.push_back(newPart);
					particles.back().Origine_x = rand() % 16;
					particles.back().Origine_y = rand() % 16;
					particles.back().vitX = rand() % 16 - 8;
					particles.back().vitY = rand() % 16 - 8;
				}

				// Adaptation des particules démarrant à la base
				for (int l = 0;l < psoParams->nbr_base;l++) {
					particles[psoParams->nbr_thread - psoParams->nbr_particule + l].Direction_tank = 'U';
					particles[psoParams->nbr_thread - psoParams->nbr_particule + l].Origine_x = mapParams->Origine_x;
					particles[psoParams->nbr_thread - psoParams->nbr_particule + l].Origine_y = mapParams->Origine_y;
				}

				// Direction et vitesse d'origine pour nouvelles particules
				for (int j = psoParams->nbr_particule; j > 0; j--) {
					particles[psoParams->nbr_thread - j].posX = particles[psoParams->nbr_thread - j].Origine_x;
					particles[psoParams->nbr_thread - j].posY = particles[psoParams->nbr_thread - j].Origine_y;

					DeplacementVitesse(&particles[psoParams->nbr_thread - j].vitX, &particles[psoParams->nbr_thread - j].vitY, &particles[psoParams->nbr_thread - j].Direction_tank, &particles[psoParams->nbr_thread - j].Output);
					particles[psoParams->nbr_thread - j].Direction_original_tank = particles[psoParams->nbr_thread - j].Direction_tank;
					Engine(mapParams, &particles[psoParams->nbr_thread - j]);
				}
			}

			// Vérification de jonction avec une particule démarrant à la base
			if (int(particles[n * psoParams->nbr_particule].distance_finish) == 0) {
				std::cout << "Chemin trouve!" << std::endl;
				found_finish = true;
				break;
			}
			if (particles[i].become_finish == false) {
				particles[i].score = n1 / particles[i].distance_finish - n2 * std::size(particles[i].Output);
				//std::cout << "score = " << score[i] << "n1 = " << n1 << "dist = " << distance_finish[i] << "n2 = " << n2 << "sizeOutput = " << std::size(Output[i]) << std::endl;
				if (particles[i].score > particles[i].score_p_best) {
					particles[i].p_bestX = particles[i].posX;
					particles[i].p_bestY = particles[i].posY;
					particles[i].score_p_best = particles[i].score;
				}
				if (particles[i].score > score_g_best[n]) {
					g_bestX[n] = particles[i].posX;
					g_bestY[n] = particles[i].posY;
					score_g_best[n] = particles[i].score;
				}
			}
		}

		// Fin du jeu si solution trouvée
		if (int(particles[n * psoParams->nbr_particule].distance_finish) == 0) break;


		random_1 = float(rand() % 100) / 100;
		random_2 = float(rand() % 100) / 100;
		//std::cout << random_1 << random_2 << std::endl;

		n = 0;
		for (int i = 0; i < psoParams->nbr_thread; i++) {
			if (i == (n + 1) * psoParams->nbr_particule) {
				n++;
			}
			if (particles[i].become_finish == false) {
				particles[i].vitX = omega * particles[i].vitX + c1 * random_1 * (particles[i].p_bestX - particles[i].posX) + c2 * random_2 * (g_bestX[n] - particles[i].posX);
				particles[i].vitY = omega * particles[i].vitY + c1 * random_1 * (particles[i].p_bestY - particles[i].posY) + c2 * random_2 * (g_bestY[n] - particles[i].posY);
				//std::cout << vitX[i] << std::endl;
				DeplacementVitesse(&particles[i].vitX, &particles[i].vitY, &particles[i].Direction_tank, &particles[i].Output);
				particles[i].Direction_tank = particles[i].Direction_original_tank;
				particles[i].posX = particles[i].Origine_x;
				particles[i].posY = particles[i].Origine_y;
				particles[i].matrice_mobile = mapParams->matrice_mobile;
				//Engine(mapParams, &particles[i]);
				//Creation de threads pour chaque particules
				thread = new std::thread(Engine, mapParams, &particles[i]);
				threads.push_back(thread);
				//std::cout << "vitX = " << vitX[i] << "omega = " << omega << "c1 = " << c1 << "random1 = " << random_1 << "pBestX = " << p_bestX[i] << "posXfin = " << posX_final[i] << "c2 = " << c2 << "random2 = " << random_2 << "g_bestX" << g_bestX << std::endl;
			}
		}

		//Attente les threads ont finis
		for (int i = 0; i < threads.size(); i++) {
			threads[i]->join();
		}

		for (int i = 0; i < threads.size(); i++) {
			delete threads[i];
		}

		threads.resize(0);

		std::cout << "Iteration " << nbr_iteration_t << std::endl;
		std::cout << "Nombre de particules : " << psoParams->nbr_thread << std::endl;
		nbr_iteration_t++;
		omega = omega - 0.03;
		if (omega < 0) omega = 0;
	}

	if (found_finish == true) {
		return finish_Output.back();
	}
	else {
		// TODO : Obtention de la meilleure solution
		return "Rien trouvé :(";
	}
}