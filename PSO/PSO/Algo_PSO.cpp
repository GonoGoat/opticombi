#include "Algo_PSO.h"

std::string Algo_PSO(mapStruct *mapParams, psoStruct *psoParams)
{
	std::vector<int> g_bestX(mapParams->nbr_arrive);
	std::vector<int> g_bestY(mapParams->nbr_arrive);
	std::vector<int> score_g_best(mapParams->nbr_arrive);
	int n1 = 150;
	int n2 = 2;
	int nbr_iteration_t = 0;

	//Variable pour la verification de la veracite de la solution
	int num_particule;

	// Identifiant d'arrivée
	int n = 0;
	bool found_finish = false;
	std::vector<std::string> finish_Output(mapParams->nbr_arrive);

	// Threads pour la parral�lisation
	std::thread *thread;
	std::vector<std::thread *> threads;

	// Création et initialisation des générateurs de nombres aléatoires
	std::random_device rd8;
	std::mt19937 rng8(rd8());
	std::uniform_int_distribution<int> dist8(0, 8);

	std::random_device rd16;
	std::mt19937 rng16(rd16());
	std::uniform_int_distribution<int> dist16(0, mapParams->nbr_case_ok-1);

	std::random_device rd100;
	std::mt19937 rng100(rd100());
	std::uniform_int_distribution<int> dist100(0, 100);

	// Initialisation des arrivées
	for (int i = 0; i < mapParams->nbr_arrive; i++)
	{
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
	for (int i = 0; i < psoParams->nbr_thread; i++)
	{
		particles.push_back(part);

		//Détection d'arrivée
		if (i == n * psoParams->nbr_particule) n++;

		// Adaptation des départs à la base
		if (i % psoParams->nbr_particule < psoParams->nbr_base) {
			particles[i].Origine_x = mapParams->Origine_x;
			particles[i].Origine_y = mapParams->Origine_y;
			particles[i].Direction_tank = mapParams->Direction_tank;
		}
		else
		{
			particles[i].Origine_x = mapParams->pos_OK_x[dist16(rng16)];
			particles[i].Origine_y = mapParams->pos_OK_y[dist16(rng16)];
			std::cout << "Particule a la position x : " << particles[i].Origine_x << " | y :" << particles[i].Origine_y << std::endl;
		}
		particles[i].vitX = dist8(rng8);
		particles[i].vitY = dist8(rng8);
		particles[i].posX = particles[i].Origine_x;
		particles[i].posY = particles[i].Origine_y;
		if (mapParams->Finish_x.size() != 0 && mapParams->Finish_y.size() != 0)
		{
			particles[i].Finish_x = mapParams->Finish_x[n - 1];
			particles[i].Finish_y = mapParams->Finish_y[n - 1];
		}

		particles[i].nbr_modifs = 0;
		DeplacementVitesse(&particles[i].vitX, &particles[i].vitY, &particles[i].Direction_tank, &particles[i].Output, IdentifierObstacle(&particles[i], psoParams->influence,mapParams), &particles[i].nbr_modifs);

		// Backup direction originale
		particles[i].Direction_original_tank = particles[i].Direction_tank;
		Engine(mapParams, &particles[i]);

		//Gestion de la mort
		if (particles[i].success == 1) {
			if (particles[i].nbr_modifs == 2) {
				particles[i].Output.pop_back();
				particles[i].Output.pop_back();
			}
			else if (particles[i].nbr_modifs == 1) particles[i].Output.pop_back();
			if (particles[i].Output == "") {
				if (i == n * psoParams->nbr_particule) particles[i].Direction_tank == 'U';
				else {
					particles[i].Direction_tank = '/';
					particles[i].Direction_original_tank = '/';
				}
			}
			else {
				particles[i].Direction_tank = particles[i].Output.back();
			}
		}
	}

	// Ittération PSO
	while (nbr_iteration_t < psoParams->nbr_iteration_max)
	{
		n = 0; // Reset du compteur d'arrivées
		for (int i = 0; i < psoParams->nbr_thread; i++)
		{
			// Changement de finish
			if (i == (n + 1) * psoParams->nbr_particule)
			{
				n++;
			}
			particles[i].distance_finish = sqrt((particles[i].Finish_x - particles[i].posX) * (particles[i].Finish_x - particles[i].posX) + (particles[i].Finish_y - particles[i].posY) * (particles[i].Finish_y - particles[i].posY));

			// Passage d'une particule comme arrivée intermédiaire
			if (int(particles[i].distance_finish) == 0 && particles[i].become_finish == false)
			{
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
					particles.back().Origine_x = mapParams->pos_OK_x[dist16(rng16)];
					particles.back().Origine_y = mapParams->pos_OK_y[dist16(rng16)];
					std::cout << "Particule a la position x : " << particles[i].Origine_x << " | y :" << particles[i].Origine_y << std::endl;
					particles.back().vitX = dist8(rng8);
					particles.back().vitY = dist8(rng8);
				}

				// Adaptation des particules démarrant à la base
				for (int l = 0;l < psoParams->nbr_base;l++) {
					particles[psoParams->nbr_thread - psoParams->nbr_particule + l].Direction_tank = 'U';
					particles[psoParams->nbr_thread - psoParams->nbr_particule + l].Origine_x = mapParams->Origine_x;
					particles[psoParams->nbr_thread - psoParams->nbr_particule + l].Origine_y = mapParams->Origine_y;
				}

				// Direction et vitesse d'origine pour nouvelles particules
				for (int j = psoParams->nbr_particule; j > 0; j--)
				{
					particles[psoParams->nbr_thread - j].posX = particles[psoParams->nbr_thread - j].Origine_x;
					particles[psoParams->nbr_thread - j].posY = particles[psoParams->nbr_thread - j].Origine_y;

					DeplacementVitesse(&particles[psoParams->nbr_thread - j].vitX, &particles[psoParams->nbr_thread - j].vitY, &particles[psoParams->nbr_thread - j].Direction_tank, &particles[psoParams->nbr_thread - j].Output, false, &particles[psoParams->nbr_thread - j].nbr_modifs);
					particles[psoParams->nbr_thread - j].Direction_original_tank = particles[psoParams->nbr_thread - j].Direction_tank;
					Engine(mapParams, &particles[psoParams->nbr_thread - j]);

					//Gestion de la mort
					if (particles[psoParams->nbr_thread - j].success == 1) {
						if (particles[psoParams->nbr_thread - j].nbr_modifs == 2) {
							particles[psoParams->nbr_thread - j].Output.pop_back();
							particles[psoParams->nbr_thread - j].Output.pop_back();
						}
						else if (particles[psoParams->nbr_thread - j].nbr_modifs == 1) particles[psoParams->nbr_thread - j].Output.pop_back();
						if (particles[psoParams->nbr_thread - j].Output == "") {
							if (i == n * psoParams->nbr_particule) particles[psoParams->nbr_thread - j].Direction_tank == 'U';
							else {
								particles[psoParams->nbr_thread - j].Direction_tank = '/';
								particles[psoParams->nbr_thread - j].Direction_original_tank = '/';
							}
						}
						else {
							particles[psoParams->nbr_thread - j].Direction_tank = particles[psoParams->nbr_thread - j].Output.back();
						}
					}
				}
			}

			// Vérification de jonction avec une particule démarrant à la base
			if (int(particles[n * psoParams->nbr_particule].distance_finish) == 0) {
				std::cout << "Chemin trouve!" << std::endl;
				found_finish = true;
				num_particule = n * psoParams->nbr_particule;
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

		psoParams->random_1 = float(dist100(rng100)) / 100;
		psoParams->random_2 = float(dist100(rng100)) / 100;
		//std::cout << random_1 << random_2 << std::endl;

		n = 0;
		std::chrono::time_point<std::chrono::system_clock> start, end;
		start = std::chrono::system_clock::now();

		//Creation de threads pour chaque univers
		//La copie du i est très importante !
		for (int i = 0; i < finish_Output.size(); i++) {
			thread = new std::thread(ParallelisationParUnivers, psoParams, &particles, mapParams, i, &g_bestX, &g_bestY);
			threads.push_back(thread);
		}
		/*
		//Creation de threads par nombre de threads logiques (4)
		for (int i = 0; i < 4; i++) {
			thread = new std::thread(ParallelisationThreadsLogiques, psoParams, &particles, mapParams, i, &g_bestX, &g_bestY);
			threads.push_back(thread);
		}*/

		//Attente les threads ont finis
		for (int i = 0; i < threads.size(); i++) {
			threads[i]->join();
		}

		for (int i = 0; i < threads.size(); i++) {
			delete threads[i];
		}

		end = std::chrono::system_clock::now();
		long long int microseconde = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		std::cout << "Temp d'execution du code " << microseconde << " microsec" << std::endl << std::endl;

		threads.clear();

		std::cout << "Iteration " << nbr_iteration_t << std::endl;
		std::cout << "Nombre de particules : " << psoParams->nbr_thread << std::endl;
		nbr_iteration_t++;
		psoParams->omega = psoParams->omega - 0.03;
		if (psoParams->omega < 0) psoParams->omega = 0;
	}

	//Verification de la veracite de la solution puis envoi de la reponse si celle-ci est bonne
	if (found_finish == true) {
		particles[num_particule].matrice_mobile = mapParams->matrice_mobile;
		particles[num_particule].Direction_tank = particles[num_particule].Direction_original_tank;
		particles[num_particule].posX = particles[num_particule].Origine_x;
		particles[num_particule].posY = particles[num_particule].Origine_y;
		particles[num_particule].Output = finish_Output.back();
		Engine(mapParams, &particles[num_particule]);

		if (particles[num_particule].success == 2) return finish_Output.back();
		else return "Solution trouvee mais fausse :( " + finish_Output.back();
	}
	else {
		return "Rien trouve :(";
	}
}