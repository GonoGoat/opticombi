#include "data.h"
#include "LogiqueDeplacementVitesse.h"
#include "Logique_jeu.h"

void ParallelisationParUnivers(psoStruct* pso_struct, std::vector<particleStruct>* particles, mapStruct* map_params, int n, std::vector<int>* g_bestX, std::vector<int>* g_bestY);
void ParallelisationThreadsLogiques(psoStruct* pso_struct, std::vector<particleStruct>* particles, mapStruct* map_params, int n, std::vector<int>* g_bestX, std::vector<int>* g_bestY);