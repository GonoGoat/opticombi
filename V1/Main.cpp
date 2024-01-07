#include "Main.h"

int main(int argc, char const *argv[])
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    //std::thread** instanciation_particule;

    // Paramètres processing
    mapStruct map;
    if (argc == 2) {
        map.nom_fichier = argv[1];
    }
    else {
        map.nom_fichier = "../Maps/Now_with_walls.lt4";
    }
    map.nbr_arrive = 0;
    map.Direction_tank = 'U';
    std::vector<std::vector<int>> matrice_fixe, matrice_mobile;

    // Paramètres Output
    outputStruct output;
    output.name = "Now_with_walls";
    output.solver = "PSO";
    // output.sequence = "UURRRRRRRRRRRRRRRRRDD"; // Temporaire
    output.output_file = "../Output/Now_with_walls.ltr";

    //std::string testSeq = "UUUUUUUUURR";
    // Paramètres particules
    std::vector<particleStruct> particles;
    
    //Paramètres PSO
    psoStruct pso;

    
    std::cout << "Combien de particules voulez-vous par arrivees ? : \n";
    std::cin >> pso.nbr_particule;

    std::cout << "Combien d'iterations maximum voulez-vous faire par solution ? : \n";
    std::cin >> pso.nbr_iteration_max;
    
    
    // Parsage de la carte
    parsage(&map);
    
    // Détection des arrivées et du départ de la carte
    detection(&map);
    
    std::cout << "Depart x : " << map.Origine_x << "   Depart y : " << map.Origine_y << std::endl;

    for (int i = 0; i < map.nbr_arrive; i++) {
        std::cout << "Position : " << i << " x =" << map.Finish_x[i] << " | y =" << map.Finish_y[i] << std::endl;
    }

    pso.nbr_thread = pso.nbr_particule * map.nbr_arrive;
    output.sequence = Algo_PSO(&map, &pso);
    std::cout << output.sequence << std::endl;
    /*
    // Paramètres dessin SVG
    svgStruct svg;
    svg.trajSuccess = 0;
    svg.output_file = "./Output/test.svg";
    svg.svgHeight = 40;

    getPositionsOfSequence(&map,&svg,&output);
    for(int i = 0;i<svg.trajX.size();i++) {
        std::cout << svg.trajX[i] << ":" << svg.trajY[i] << std::endl;
    }
    drawSVG(&map,&svg);*/

    // Engine(&map, &part);

    create_ltr_file(&output);
    end = std::chrono::system_clock::now();
    long long int microseconde = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Temp d'execution du code " << microseconde << " microsec" << std::endl << std::endl;

    return 0;
}