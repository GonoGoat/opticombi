#include "Main.h"

int main()
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    //std::thread** instanciation_particule;

    //Paramètres Parsage
    std::string nom_fichier = "./Maps/Trajectoire.lt4";
    std::vector<std::vector<int>> matrice_fixe, matrice_mobile;
    int nbr_arrive = 0;
    int nombreLignes, nombreColonnes;
    //Paramètres Deplacement
    int Origine_x, Origine_y;
    std::vector<int> Finish_x, Finish_y = {};
    char Direction_tank = 'U';
    //Paramètres LTR
    std::string name = "One block into the water";
    std::string solver = "PSO";
    std::string sequence = "FLLUFLLUFLLLUFURRRRRRRRUFRRUFRRRUF";
    std::string output_file = "5_arrive.ltr";
    //Paramètres instanciations
    int nbr_particule;
    int nbr_instance;
    int nbr_thread;
    //Paramètres PSO
    int nbr_iteration_max;
    //paramètres engine
    int success = 0;

    /*std::cout << "Combien d'instance voulez-vous generer ? : ";
    std::cin >> nbr_instance;*/

    // Import matrice
    parsageParam params;
    params.nom_fichier = &nom_fichier;
    params.matrice_fixe = &matrice_fixe;
    params.matrice_mobile = &matrice_mobile;
    params.nombreLignes = &nombreLignes;
    params.nombreColonnes = &nombreColonnes;

    std::cout << "Combien de particules voulez-vous par arrivees ? : \n";
    std::cin >> nbr_particule;

    std::cout << "Combien d'iterations maximum voulez-vous faire par solution ? : \n";
    std::cin >> nbr_iteration_max;
    
    parsage(&params);
    
    //parsage("..\\..\\"+nom_fichier,matrice);
    // detection(&matrice_fixe, &nombreLignes, &nombreColonnes, &Origine_x, &Origine_y, &Finish_x, &Finish_y, &nbr_arrive);

    // Etablissement du trajet
    std::vector<int> trajX = {Origine_x}; // Historique des positions X
    std::vector<int> trajY = {Origine_y}; // Historique des positions Y
    int trajSuccess = 0; // Succès de la séquence

    nbr_thread = nbr_particule * nbr_arrive;
    //sequence = Algo_PSO(&matrice, &nbr_thread, &nbr_iteration_max, &Origine_x, &Origine_y, &Finish_x, &Finish_y, &nbr_particule);
    std::cout << sequence << std::endl;

    getPositionsOfSequence(&matrice_fixe, sequence, &trajX, &trajY, &trajSuccess);
    /*for(int i = 0;i<trajX.size();i++) {
        std::cout << trajX[i] << ":" << trajY[i] << std::endl;
    }*/
    // drawSVG(&matrice,Origine_x, Origine_y, &Finish_x, &Finish_y, nbr_arrive, sequence, "./Output/Sequence.svg",nombreLignes, nombreColonnes, &trajX, &trajY, trajSuccess);

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
    
    /*std::cout << "Depart x : " << Origine_x << "   Depart y : " << Origine_y << std::endl;

    for (int i = 0; i < nbr_arrive; i++) {
        std::cout << "Position : " << i << " x =" << Finish_x[i] << " | y =" << Finish_y[i] << std::endl;
    }

    Engine(matrice, sequence, &Origine_x, &Origine_y, &Direction_tank, &success);*/

    create_ltr_file(name, solver, sequence, output_file);
    end = std::chrono::system_clock::now();
    long long int microseconde = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Temp d'execution du code " << microseconde << " microsec" << std::endl << std::endl;

    /*for (int i = 0; i < nbr_instance; i++) {
        delete[] instanciation_particule[i];
    }
    delete[] instanciation_particule;

    std::cin >> nbr_instance;*/
    return 0;
}