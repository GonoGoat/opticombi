#include "Parall.h"

void ParallelisationParUnivers(psoStruct* pso_struct, std::vector<particleStruct>* particles, mapStruct* map_params, int n, std::vector<int>* g_bestX, std::vector<int>* g_bestY) {

	for (int i = 0; i < pso_struct->nbr_particule; i++) {
		if (particles[0][(n * pso_struct->nbr_particule) + i].become_finish == false) {
			particles[0][(n * pso_struct->nbr_particule) + i].vitX = pso_struct->omega * particles[0][(n * pso_struct->nbr_particule) + i].vitX + pso_struct->c1 * pso_struct->random_1 * (particles[0][(n * pso_struct->nbr_particule) + i].p_bestX - particles[0][(n * pso_struct->nbr_particule) + i].posX) + pso_struct->c2 * pso_struct->random_2 * (g_bestX[0][n] - particles[0][(n * pso_struct->nbr_particule) + i].posX);
			particles[0][(n * pso_struct->nbr_particule) + i].vitY = pso_struct->omega * particles[0][(n * pso_struct->nbr_particule) + i].vitY + pso_struct->c1 * pso_struct->random_1 * (particles[0][(n * pso_struct->nbr_particule) + i].p_bestY - particles[0][(n * pso_struct->nbr_particule) + i].posY) + pso_struct->c2 * pso_struct->random_2 * (g_bestY[0][n] - particles[0][(n * pso_struct->nbr_particule) + i].posY);

			particles[0][(n * pso_struct->nbr_particule) + i].nbr_modifs = 0;
			DeplacementVitesse(&particles[0][(n * pso_struct->nbr_particule) + i].vitX, &particles[0][(n * pso_struct->nbr_particule) + i].vitY, &particles[0][(n * pso_struct->nbr_particule) + i].Direction_tank, &particles[0][(n * pso_struct->nbr_particule) + i].Output, IdentifierObstacle(&particles[0][(n * pso_struct->nbr_particule) + i], pso_struct->influence,map_params), &particles[0][(n * pso_struct->nbr_particule) + i].nbr_modifs);
			if (particles[0][(n * pso_struct->nbr_particule) + i].Direction_original_tank == '/') {
				particles[0][(n * pso_struct->nbr_particule) + i].Direction_original_tank = particles[0][(n * pso_struct->nbr_particule) + i].Output.at(0);
			}
			particles[0][(n * pso_struct->nbr_particule) + i].matrice_mobile = map_params->matrice_mobile;
			particles[0][(n * pso_struct->nbr_particule) + i].Direction_tank = particles[0][(n * pso_struct->nbr_particule) + i].Direction_original_tank;
			particles[0][(n * pso_struct->nbr_particule) + i].posX = particles[0][(n * pso_struct->nbr_particule) + i].Origine_x;
			particles[0][(n * pso_struct->nbr_particule) + i].posY = particles[0][(n * pso_struct->nbr_particule) + i].Origine_y;
			Engine(map_params, &(*particles)[(n * pso_struct->nbr_particule) + i]);

			//Gestion de la mort
			if (particles[0][(n * pso_struct->nbr_particule) + i].success == 1) {
				if (particles[0][(n * pso_struct->nbr_particule) + i].nbr_modifs == 2) {
					particles[0][(n * pso_struct->nbr_particule) + i].Output.pop_back();
					particles[0][(n * pso_struct->nbr_particule) + i].Output.pop_back();
				}
				else if (particles[0][(n * pso_struct->nbr_particule) + i].nbr_modifs == 1) particles[0][(n * pso_struct->nbr_particule) + i].Output.pop_back();
				if (particles[0][(n * pso_struct->nbr_particule) + i].Output == "") {
					if (i == n * pso_struct->nbr_particule) particles[0][(n * pso_struct->nbr_particule) + i].Direction_tank == 'U';
					else {
						particles[0][(n * pso_struct->nbr_particule) + i].Direction_tank = '/';
						particles[0][(n * pso_struct->nbr_particule) + i].Direction_original_tank = '/';
					}
				}
				else {
					particles[0][(n * pso_struct->nbr_particule) + i].Direction_tank = particles[0][(n * pso_struct->nbr_particule) + i].Output.back();
				}
			}
			particles[0][(n * pso_struct->nbr_particule) + i].success = 0;
		}
	}
}

void ParallelisationThreadsLogiques(psoStruct* pso_struct, std::vector<particleStruct>* particles, mapStruct* map_params, int n, std::vector<int>* g_bestX, std::vector<int>* g_bestY) {
	int nbr_particules_thread = pso_struct->nbr_thread / 4;
	int nbre = pso_struct->nbr_particule;
	int n_univers = 0;

	for (int i = 0; i < nbr_particules_thread; i++) {
		while (((n * nbr_particules_thread + nbr_particules_thread) - 1) > nbre) {
			n_univers++;
			nbre += nbre;
		}

		if (particles[0][(n * nbr_particules_thread) + i].become_finish == false) {

			particles[0][(n * nbr_particules_thread) + i].vitX = pso_struct->omega * particles[0][(n * nbr_particules_thread) + i].vitX + pso_struct->c1 * pso_struct->random_1 * (particles[0][(n * nbr_particules_thread) + i].p_bestX - particles[0][(n * nbr_particules_thread) + i].posX) + pso_struct->c2 * pso_struct->random_2 * (g_bestX[0][n_univers] - particles[0][(n * nbr_particules_thread) + i].posX);
			particles[0][(n * nbr_particules_thread) + i].vitY = pso_struct->omega * particles[0][(n * nbr_particules_thread) + i].vitY + pso_struct->c1 * pso_struct->random_1 * (particles[0][(n * nbr_particules_thread) + i].p_bestY - particles[0][(n * nbr_particules_thread) + i].posY) + pso_struct->c2 * pso_struct->random_2 * (g_bestY[0][n_univers] - particles[0][(n * nbr_particules_thread) + i].posY);

			particles[0][(n * nbr_particules_thread) + i].nbr_modifs = 0;
			DeplacementVitesse(&particles[0][(n * nbr_particules_thread) + i].vitX, &particles[0][(n * nbr_particules_thread) + i].vitY, &particles[0][(n * nbr_particules_thread) + i].Direction_tank, &particles[0][(n * nbr_particules_thread) + i].Output, IdentifierObstacle(&particles[0][(n * pso_struct->nbr_particule) + i], pso_struct->influence,map_params), &particles[0][(n * nbr_particules_thread) + i].nbr_modifs);
			if (particles[0][(n * nbr_particules_thread) + i].Direction_original_tank == '/') {
				particles[0][(n * nbr_particules_thread) + i].Direction_original_tank = particles[0][(n * nbr_particules_thread) + i].Output.at(0);
			}
			particles[0][(n * nbr_particules_thread) + i].Direction_tank = particles[0][(n * nbr_particules_thread) + i].Direction_original_tank;
			particles[0][(n * nbr_particules_thread) + i].posX = particles[0][(n * nbr_particules_thread) + i].Origine_x;
			particles[0][(n * nbr_particules_thread) + i].posY = particles[0][(n * nbr_particules_thread) + i].Origine_y;
			Engine(map_params, &(*particles)[(n * nbr_particules_thread) + i]);
			if ((*particles)[(n * nbr_particules_thread) + i].posX >= map_params->nbr_colonnes || (*particles)[(n * nbr_particules_thread) + i].posY >= map_params->nbr_lignes)  {
                std::cout << "Dépassement !\n" ;
            }

			//Gestion de la mort
			if (particles[0][(n * nbr_particules_thread) + i].success == 1) {
				if (particles[0][(n * nbr_particules_thread) + i].nbr_modifs == 2) {
					particles[0][(n * nbr_particules_thread) + i].Output.pop_back();
					particles[0][(n * nbr_particules_thread) + i].Output.pop_back();
				}
				else if (particles[0][(n * nbr_particules_thread) + i].nbr_modifs == 1) particles[0][(n * nbr_particules_thread) + i].Output.pop_back();
				if (particles[0][(n * nbr_particules_thread) + i].Output == "") {
					if (i == n * pso_struct->nbr_particule) particles[0][(n * nbr_particules_thread) + i].Direction_tank == 'U';
					else {
						particles[0][(n * nbr_particules_thread) + i].Direction_tank = '/';
						particles[0][(n * nbr_particules_thread) + i].Direction_original_tank = '/';
					}
				}
				else {
					particles[0][(n * nbr_particules_thread) + i].Direction_tank = particles[0][(n * nbr_particules_thread) + i].Output.back();
				}
			}
		}
	}
}