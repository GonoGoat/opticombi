#include "Parseur.h"
#include "LogiqueDeplacement.h"
#include "LTRcreator.h"
#include "Detection_Start_End.h"
#include "Algo_PSO.h"
#include <thread>

int main()
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    //std::thread** instanciation_particule;

    //Paramètres Parsage
    std::string nom_fichier = "Full_Dirt.lt4";
    std::string matrice[16][16];
    int nbr_arrive = 0;
    //Paramètres Deplacement
    int Origine_x;
    int Origine_y;
    int Finish_x[5] = { 0,0,0,0,0 };
    int Finish_y[5] = { 0,0,0,0,0 };
    //Paramètres LTR
    std::string name = "Full_Dirt";
    std::string solver = "PSO";
    std::string sequence;
    std::string output_file = "Full_Dirt.ltr";
    //Paramètres instanciations
    int nbr_particule;
    int nbr_instance;
    int nbre_thread;
    //Paramètres PSO
    int nbr_iteration_max;
    std::string Output;

    std::cout << "Combien d'instance voulez-vous generer ? : \n";
    std::cin >> nbr_instance;

    std::cout << "Combien d'iterations maximum voulez-vous faire par solution ? : \n";
    std::cin >> nbr_iteration_max;
      

    parsage("..\\..\\"+nom_fichier,matrice);
    detection(matrice, &Origine_x, &Origine_y, Finish_x, Finish_y, &nbr_arrive);

    nbr_particule = nbr_instance/nbr_arrive;
    std::cout << "Nombre de particules par arrivees : " << nbr_particule << std::endl;

    nbre_thread = nbr_particule * nbr_arrive;
    Output = Algo_PSO(&nbre_thread, &nbr_iteration_max, &Origine_x, &Origine_y, Finish_x, Finish_y);
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

    std::cout << Output << std::endl;

    /*for (int i = 0; i < nbr_thread; i++) {
        delete[] instanciation_particule[i];
    }
    delete[] instanciation_particule;*/

    return 0;
}
