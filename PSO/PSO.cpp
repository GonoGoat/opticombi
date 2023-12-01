#include "Parseur.h"
#include "LogiqueDeplacement.h"
#include "LTRcreator.h"
#include "Detection_Start_End.h"
#include "Logique_jeu.h"
#include "sequenceToSVG.h"
#include "Algo_PSO.h"
#include <vector>
#include <thread>

int main()
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    //std::thread** instanciation_particule;

    //Paramètres Parsage
    std::string nom_fichier = "Now_with_walls.lt4";
    std::vector<std::vector<int>> matrice;
    int nbr_arrive = 0;
    //Paramètres Deplacement
    int Origine_x;
    int Origine_y;
    int nombreLignes;
    int nombreColonnes = 1;
    std::vector<int> Finish_x;
    std::vector<int> Finish_y;
    //int Finish_x[5] = { 0,0,0,0,0 };
    //int Finish_y[5] = { 0,0,0,0,0 };
    //Paramètres LTR
    std::string name = "Now_with_walls";
    std::string solver = "PSO";
    std::string sequence = "";
    std::string output_file = "Now_with_walls.ltr";
    //Paramètres instanciations
    int nbr_particule;
    int nbre_thread;
    //Paramètres PSO
    int nbr_iteration_max;
    //paramètres engine
    int success = 0;

    std::cout << "Combien de particules voulez-vous par arrivees ? : \n";
    std::cin >> nbr_particule;

    std::cout << "Combien d'iterations maximum voulez-vous faire par solution ? : \n";
    std::cin >> nbr_iteration_max;

    parsage("../Maps/" + nom_fichier, &matrice, &nombreLignes, &nombreColonnes);
    detection(matrice, nombreLignes, nombreColonnes, &Origine_x, &Origine_y, &Finish_x, &Finish_y, &nbr_arrive);

    // Etablissement du trajet
    std::vector<int> trajX = { Origine_x }; // Historique des positions X
    std::vector<int> trajY = { Origine_y }; // Historique des positions Y
    int trajSuccess = 0;

    nbre_thread = nbr_particule * nbr_arrive;
    sequence = Algo_PSO(&matrice, &nbre_thread, &nbr_iteration_max, &Origine_x, &Origine_y, &Finish_x, &Finish_y, &nbr_particule);
    std::cout << sequence << std::endl;

    getPositionsOfSequence(&matrice, sequence, &trajX, &trajY, &trajSuccess);
    drawSVG(matrice, Origine_x, Origine_y, Finish_x, Finish_y, nbr_arrive, sequence, "../Output/Sequence.svg", nombreLignes, nombreColonnes, trajX, trajY, trajSuccess);
    /*instanciation_particule = new std::thread* [nbr_thread-1];

    for (int j = 0; j < nbr_particule; j++) {
        for (int i = 0; i < nbr_arrive; i++) {
            //std::cout << "instance numero " << nbr_arrive * j + i << std::endl;
            instanciation_particule[nbr_arrive * j + i] = new std::thread(Deplacement, &Origine_x, &Origine_y, Finish_x, Finish_y, &Direction_tank, &sequence, &i);
        }
    }
    
    for (int i = 0; i < nbr_thread; i++) {
        instanciation_particule[i] -> join();
    }*/

    create_ltr_file(name, solver, sequence, output_file);
    end = std::chrono::system_clock::now();
    long long int microseconde = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    /*std::cout << "Temp d'execution du code " << microseconde << " microsec" << std::endl << std::endl;

    std::cout << "Depart x : " << Origine_x << "   Depart y : " << Origine_y << std::endl;

    for (int i = 0; i < nbr_arrive; i++) {
        std::cout << "Position : " << i << " x =" << Finish_x[i] << " | y =" << Finish_y[i] << std::endl;
    }*/

    //std::cout << sequence << std::endl;

    /*for (int i = 0; i < nbr_thread; i++) {
        delete[] instanciation_particule[i];
    }
    delete[] instanciation_particule;*/

    return 0;
}
