#include <iostream>
#include <string>
#include <vector>
#include "data.h"

void getPositionsOfSequence (mapStruct* mapParams, svgStruct* svgParams, outputStruct* outputParams);
void Engine(mapStruct* mapParams, outputStruct* outputParams);
void Deplacement(moveStruct* moveParams);
void Verification_deplacement (mapStruct* mapParams, moveStruct* moveParams);
void Portail (mapStruct* mapParams, moveStruct* moveParams);
void Verification_Anti_Tank(mapStruct* mapParams, moveStruct* moveParams);
void Verification_Anti_Tank_parcour_vertical(mapStruct* mapParams, moveStruct* moveParams);
void Verification_Anti_Tank_parcour_horizontal(mapStruct* mapParams, moveStruct* moveParams);
void Tir(mapStruct* mapParams, moveStruct* moveParams);
void glace(mapStruct* mapParams, moveStruct* moveParams);
void glace_fine(mapStruct* mapParams, moveStruct* moveParams);
void path(mapStruct* mapParams, moveStruct* moveParams);