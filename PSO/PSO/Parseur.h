#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>

enum Matrice
{
	Dirt,
	Tank,
	Base,
	Water,
	Sollid_Block,
	Movable_Block,
	Bricks,
	Anti_Tank_U, Anti_Tank_D, Anti_Tank_R, Anti_Tank_L,
	Mirror_UR, Mirror_UL, Mirror_DR, Mirro_DL,
	Way_U, Way_D, Way_R, Way_L,
	Crystal_Block,
	Rotative_Mirror_UR, Rotative_Mirror_UL, Rotative_Mirror_DR, Rotative_Mirror_DL,
	Ice,
	Thin_Ice,
	Tunnel_Red, Tunnel_Green, Tunnel_Blue, Tunnel_Cyan, Tunnel_Yellow, Tunnel_Pink, Tunnel_White, Tunnel_Dark

};

void parsage(std::string nom_fichier, int matrice[16][16]);
Matrice conversion(const std::string& caractere);