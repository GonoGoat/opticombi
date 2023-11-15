#include "Parseur.h"
#include "LogiqueDeplacement.h"
#include "LTRcreator.h"
#include "Detection_Start_End.h"
#include "Logique_Jeu.h"
#include <thread>

int main()
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    std::thread** instanciation_particule;

    //Paramètres Parsage
    std::string nom_fichier = "Murs_limite_portail.lt4";
    int matrice[16][16];
    int nbr_arrive = 0;
    //Paramètres Deplacement
    int Origine_x;
    int Origine_y;
    int Finish_x[5] = { 0,0,0,0,0 };
    int Finish_y[5] = { 0,0,0,0,0 };
    char Direction_tank = 'U';
    //Paramètres LTR
    std::string name = "One block into the water";
    std::string solver = "PSO";
    std::string sequence = "DDRRRRRRUURRRUUULLLUULLLLLLLUUULLLLLUUUUULLLLRRRLLRRRUUULLL";
    std::string output_file = "5_arrive.ltr";
    //Paramètres instanciations
    int nbr_particule;
    int nbr_instance;
    int nbr_thread;
    //paramètres engine
    bool success = false;

    /*std::cout << "Combien d'instance voulez-vous generer ? : ";
    std::cin >> nbr_instance;*/

    parsage("..\\..\\"+nom_fichier,matrice);
    detection(matrice, &Origine_x, &Origine_y, Finish_x, Finish_y, &nbr_arrive);

    /*nbr_particule = nbr_instance/nbr_arrive;
    std::cout << "Nombre de particule : " << nbr_particule << std::endl;
    nbr_thread = nbr_particule * nbr_arrive;*/

    /*instanciation_particule = new std::thread* [nbr_thread-1];

    for (int j = 0; j < nbr_particule; j++) {
        for (int i = 0; i < nbr_arrive; i++) {
            std::cout << "instance numero " << nbr_arrive * j + i << std::endl;
            instanciation_particule[nbr_arrive * j + i] = new std::thread(Deplacement, &Origine_x, &Origine_y, Finish_x, Finish_y, &Direction_tank, &sequence, &i);
        }
    }
    
    for (int i = 0; i < nbr_thread; i++) {
        instanciation_particule[i] -> join();
    }*/

    //create_ltr_file(name, solver, sequence, output_file);
    /*end = std::chrono::system_clock::now();
    long long int microseconde = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Temp d'execution du code " << microseconde << " microsec" << std::endl << std::endl;*/

    std::cout << "Depart x : " << Origine_x << "   Depart y : " << Origine_y << std::endl;

    for (int i = 0; i < nbr_arrive; i++) {
        std::cout << "Position : " << i << " x =" << Finish_x[i] << " | y =" << Finish_y[i] << std::endl;
    }

    Engine(matrice, sequence, &Origine_x, &Origine_y, &Direction_tank, &success);

    /*for (int i = 0; i < nbr_instance; i++) {
        delete[] instanciation_particule[i];
    }
    delete[] instanciation_particule;

    std::cin >> nbr_instance;*/
    return 0;
}
