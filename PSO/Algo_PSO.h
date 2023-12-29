#include <math.h>
#include <random>
#include <string.h>
#include <thread>
#include "LogiqueDeplacementVitesse.h"
#include "Logique_jeu.h"

std::string Algo_PSO(std::vector<std::vector<int>>* matrice, int* nbre_thread, int* nbr_iteration_max, int* Origine_x, int* Origine_y, std::vector<int>* Finish_x, std::vector<int>* Finish_y, int* nbr_particule);