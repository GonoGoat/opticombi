#include <iostream>
#include <string>
#include <vector>
#include "data.h"

void getPositionsOfSequence (mapStruct* mapParams, svgStruct* svgParams, outputStruct* outputParams, particleStruct* partParams);
void Engine(mapStruct* mapParams,  particleStruct* partParams);
void Deplacement(char* dir, int* pos_x, int* pos_y);
void Verification_deplacement (mapStruct* mapParams, moveStruct* moveParams, particleStruct* partParams);
void Portail (mapStruct* mapParams, moveStruct* moveParams, particleStruct* partParams);
void Verification_Anti_Tank(mapStruct* mapParams, moveStruct* moveParams, particleStruct* partParams);
void Verification_Anti_Tank_parcour_vertical(mapStruct* mapParams, moveStruct* moveParams, particleStruct* partParams);
void Verification_Anti_Tank_parcour_horizontal(mapStruct* mapParams, moveStruct* moveParams, particleStruct* partParams);
void Tir(mapStruct* mapParams, moveStruct* moveParams, particleStruct* partParams);
void glace(mapStruct* mapParams, moveStruct* moveParams, particleStruct* partParams);
void glace_fine(mapStruct* mapParams, moveStruct* moveParams, particleStruct* partParams);
void path(mapStruct* mapParams, moveStruct* moveParams, particleStruct* partParams);
void tir_Anti_Tank(mapStruct* mapParams, moveStruct* moveParams, particleStruct* partParams);
void inverserDirection(char* direction);