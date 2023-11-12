/*#include "./utils/Parseur.cpp"

int main()
{
    parsage("Maps/Beginner-II.lt4");

}*/

#include "./utils/Parseur.cpp"
#include "./utils/LogiqueDeplacement.cpp"
#include "./utils/LTRcreator.cpp"
#include "./utils/Detection_Start_End.cpp"
#include <thread>

int main()
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    std::thread** instanciation_particule;

    //Paramètres Parsage
    std::string nom_fichier = "./Maps/Beginner-I.lt4";
    std::string matrice[16][16];
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
    std::string sequence;
    std::string output_file = "./Output/51_arrive.svg";
    //Paramètres instanciations
    int nbr_particule;
    int nbr_instance;
    int nbr_thread;
    // Données de l'essaim
    // TODO

    std::cout << "Combien d'instance voulez-vous generer ? : ";
    std::cin >> nbr_instance;

    // Import de la carte
    parsage(nom_fichier,matrice);

    // Détection des arrivées et du point de départ
    detection(matrice, &Origine_x, &Origine_y, Finish_x, Finish_y, &nbr_arrive);

    // Adaptation du nombre de particules (threads) au nombre d'arrivées trouvées
    nbr_particule = nbr_instance/nbr_arrive;
    std::cout << "Nombre de particule : " << nbr_particule << std::endl;
    nbr_thread = nbr_particule * nbr_arrive;

    // Création des particules
    /*
    instanciation_particule = new std::thread* [nbr_thread-1];
    for (int j = 0; j < nbr_particule; j++) {
        for (int i = 0; i < nbr_arrive; i++) {
            std::cout << "instance numero " << nbr_arrive * j + i << std::endl;
            instanciation_particule[nbr_arrive * j + i] = new std::thread(Deplacement, &Origine_x, &Origine_y, Finish_x, Finish_y, &Direction_tank, &sequence, &i);
        }
    }
    for (int i = 0; i < nbr_thread; i++) {
        instanciation_particule[i] -> join();
    }*/

    // PSO
    // TODO

    // Création de la séquence de sortie
    create_ltr_file(name, solver, sequence, output_file);
    end = std::chrono::system_clock::now();
    long long int microseconde = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Temp d'execution du code " << microseconde << " microsec" << std::endl << std::endl;

    std::cout << "Depart x : " << Origine_x << "   Depart y : " << Origine_y << std::endl;

    for (int i = 0; i < nbr_arrive; i++) {
        std::cout << "Position : " << i << " x =" << Finish_x[i] << " | y =" << Finish_y[i] << std::endl;
    }

    // Garbage Collector
    for (int i = 0; i < nbr_instance; i++) {
        delete[] instanciation_particule[i];
    }
    delete[] instanciation_particule;

    std::cin >> nbr_instance;
    return 0;
}