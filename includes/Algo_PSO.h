#include <math.h>
#include <random>
#include <string.h>
#include <thread>
#include "LogiqueDeplacementVitesse.h"
#include "Logique_jeu.h"
#include "IdentifierObstacle.h"
#include "Parall.h"
#include "data.h"

particleStruct Algo_PSO(mapStruct* mapParams, psoStruct* psoParams);
particleStruct Algo_PSO_bis(mapStruct* mapParams, psoStruct* psoParams, particleStruct* partParams);
int getBestParticleAlive(std::vector<particleStruct>* particles, int* nbr_base);