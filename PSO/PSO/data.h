#include <unordered_map>
#include <string>
#include <vector>
#include <regex>

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
    Anti_Tank_Dead_U, Anti_Tank_Dead_D, Anti_Tank_Dead_R, Anti_Tank_Dead_L,
    Mirror_UR, Mirror_UL, Mirror_DR, Mirror_DL,
    Way_U, Way_D, Way_R, Way_L,
    Crystal_Block,
    Rotative_Mirror_UR, Rotative_Mirror_UL, Rotative_Mirror_DR, Rotative_Mirror_DL,
    Ice,
    Thin_Ice,
    Tunnel_Red, Tunnel_Green, Tunnel_Blue, Tunnel_Cyan, Tunnel_Yellow, Tunnel_Pink, Tunnel_White, Tunnel_Dark,
    Bridge

};

enum Success {
    En_vie,
    Mort,
    Base_atteinte,
    Position_non_valide,
    Passage_chemin,
    Portail_bloque
};

// A splitter avec particleStruct quand parallèle
struct mapStruct {
    std::string nom_fichier;
    std::string nom_map;
	std::vector<std::vector<int>> matrice_fixe, matrice_mobile;
    int nbr_arrive;
    int nbr_lignes;
    int nbr_colonnes;
    
    // Coordonnées du point de départ du tank sur la carte
    int Origine_x,Origine_y;
    
    // Ensemble des coordonnées d'arrivées possibles sur la carte
    std::vector<int> Finish_x, Finish_y; 
    char Direction_tank;

    // Ensemble des positions possibles sur la carte
    std::vector<int> pos_OK_x, pos_OK_y;
    int nbr_case_ok;
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
    // Nombre de positions initalisées
    int nbr_particule;

    // Nombre de trajets uniques
    int nbr_thread;

    // Durée de vie d'une particule en nombre d'itération
    int nbr_iteration_max;

    // Pourcentage de particule démarrant à la base de la carte
    int nbr_base;

    //Parametres PSO
    float omega = 0, c1 = 1, c2 = 1, random_1, random_2;

    // Influence tirer ou bouger (plus cette valeur se rapproche de 1, plus il va avoir tendance à tirer)
	double influence;
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

struct particleStruct {
    std::vector<std::vector<int>> matrice_mobile;

    // Position de départ de la particule
    int Origine_x, Origine_y;

    // Coordonnées de l'arrivée que la particule essaie de joindre
    int Finish_x, Finish_y;
    char Direction_tank, Direction_original_tank;

    // Indique si tank est arrivée à la fin(1), est en vie(0) ou est mort 
    int success;

    // Coordonnées de travail
    int posX,posY;
    std::string Output;

    //Taille séquence exécuté
    int taille_sequence;

    // Algorithmie
    float vitX, vitY;
    int p_bestX, p_bestY;
    int score_p_best;
    int score;
    bool become_finish;
    float distance_finish;
    int nbr_modifs;
};

struct mobileStruct {
    // Position envisagée
    int depl_x, depl_y;

    // Position origine bloc
    int Origine_x, Origine_y;

    // Sens du laser
    char dir;
};

extern std::unordered_map<std::string, Matrice> conversionToEnum;

#endif