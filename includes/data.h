#include <unordered_map>
#include <string>
#include <vector>

#ifndef DATA_H
#define DATA_H

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
    Tunnel_Red, Tunnel_Green, Tunnel_Blue, Tunnel_Cyan, Tunnel_Yellow, Tunnel_Pink, Tunnel_White, Tunnel_Dark,
    Bridge

};

// A splitter avec particleStruct quand parallèle
struct mapStruct {
    std::string nom_fichier;
	std::vector<std::vector<int>> matrice_fixe;
    std::vector<std::vector<int>> matrice_mobile;
    int nbr_arrive;
    int nbr_lignes;
    int nbr_colonnes;
    
    // Coordonnées du point de départ du tank sur la carte
    int Origine_x,Origine_y;

    // Coordonnées de travail
    int posX,posY;
    
    // Ensemble des coordonnées d'arrivées possibles sur la carte
    std::vector<int> Finish_x, Finish_y; 
    char Direction_tank;

    // Indique si tank est arrivée à la fin(1), est en vie(0) ou est mort 
    int success;
};

struct outputStruct {
    // Nom de la solution
    std::string name;

    // Nom du solver
    std::string solver; 

    // Séquence 
    std::string sequence;

    // Nom du fichier de sortie (extension .lt4)
    std::string output_file; 
};

struct psoStruct {
    int nbr_particule;

    // Nombre de positions initalisées
    int nbr_instance;
    int nbr_thread;

    // Durée de vie d'une particule en nombre d'itération
    int nbr_iteration_max;
};

struct svgStruct {
    // Historique des coordonnées
    std::vector<int> trajX, trajY; 

    // Etat du tank
    int trajSuccess;

    // Nom du fichier de sortie (extension .svg)
    std::string output_file;

    // Taille du SVG - A adapter à votre écran
    int svgHeight;
};

struct moveStruct {
    // Position envisagée
    int depl_x, depl_y;

    // Sens du tank
    char dir;
};

// TODO : particleStruct
// matrice_mobile
// start_posX/Y
// success
// finish_X/Y
// dir

extern std::unordered_map<std::string, Matrice> conversionToEnum;

#endif