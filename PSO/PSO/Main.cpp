#include "Main.h"

int main()
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    //std::thread** instanciation_particule;

    // Paramètres processing
    mapStruct map;
    map.nom_fichier = "..\\..\\Trajectoire_Anti_Tank_4.lt4";
    map.nbr_arrive = 0;
    map.Direction_tank = 'U';
    map.success = 0;
    std::vector<std::vector<int>> matrice_fixe, matrice_mobile;

    // Paramètres Output
    outputStruct output;
    output.name = "One block into the water";
    output.solver = "PSO";
    output.sequence = "U"; // Temporaire
    output.output_file = "./Output/new.ltr";
    
    //Paramètres PSO
    psoStruct pso;

    // Paramètres dessin SVG
    svgStruct svg;
    svg.trajSuccess = 0;
    svg.output_file = "./Output/test.svg";
    svg.svgHeight = 40;

    /*std::cout << "Combien d'instance voulez-vous generer ? : ";
    std::cin >> nbr_instance;*/

    // Import matrice
    /*
    std::cout << "Combien de particules voulez-vous par arrivees ? : \n";
    std::cin >> pso.nbr_particule;

    std::cout << "Combien d'iterations maximum voulez-vous faire par solution ? : \n";
    std::cin >> pso.nbr_iteration_max;*/
    
    // Parsage de la carte
    parsage(&map);
    
    // Détection des arrivées et du départ de la carte
    detection(&map);

    // TODO : Assignation des positions de travail (Une partie aléatoire et certains sur départ)
    map.posX = map.Origine_x;
    map.posY = map.Origine_y;

    //nbr_thread = nbr_particule * nbr_arrive;
    //sequence = Algo_PSO(&matrice, &nbr_thread, &nbr_iteration_max, &Origine_x, &Origine_y, &Finish_x, &Finish_y, &nbr_particule);
    //std::cout << sequence << std::endl;

    // Etablissement du trajet
    /*getPositionsOfSequence(&map,&svg,&output);
    for(int i = 0;i<svg.trajX.size();i++) {
        std::cout << svg.trajX[i] << ":" << svg.trajY[i] << std::endl;
    }
    drawSVG(&map,&svg);*/

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
    std::cout << "Depart x : " << map.Origine_x << "   Depart y : " << map.Origine_y << std::endl;

    for (int i = 0; i < map.nbr_arrive; i++) {
        std::cout << "Position : " << i << " x =" << map.Finish_x[i] << " | y =" << map.Finish_y[i] << std::endl;
    }

    Engine(&map, &output);

    //create_ltr_file(&output);
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