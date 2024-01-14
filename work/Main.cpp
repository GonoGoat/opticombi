#include "Main.h"


int main(int argc, char const* argv[])
{
    int nbr_particule;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    // std::thread** instanciation_particule;

    // Paramètres processing
    mapStruct map;
    if (argc == 2)
    {
        map.nom_fichier = argv[1];
    }
    else
    {
        map.nom_fichier = "./Maps/prev/Chemin.lt4";
    }
    map.nbr_arrive = 0;
    map.Direction_tank = 'U';
    std::vector<std::vector<int>> matrice_fixe, matrice_mobile;

    // Paramètres Output
    outputStruct output;
    output.solver = "PSO";
    // output.sequence = "UURRRRRRRRRRRRRRRRRDD"; // Temporaire

    //std::string testSeq = "UUUUUUUUURR";
    
    //Paramètres PSO
    psoStruct pso;
    pso.influence = 0.7;
    pso.deadline_prim = 30;
    pso.deadline_bis = 30;

    std::cout << "Combien de particules voulez-vous par arrivees ? : \n";
    std::cin >> nbr_particule;
    pso.nbr_particule = nbr_particule;
    pso.nbr_base = floor(pso.nbr_particule*0.1);

    std::cout << "Combien d'iterations maximum voulez-vous faire par solution ? : \n";
    std::cin >> pso.nbr_iteration_max;

    // Parsage de la carte
    parsage(&map);
    output.name = map.nom_map;
    std::regex rgx(R"([/\\][a-zA-Z_-]+\.lt4)");
    std::smatch match;

    if (std::regex_search(map.nom_fichier, match, rgx))
        output.output_file = "./Output/" + match[0].str().substr(1,match[0].str().size()-5) + ".ltr";
    else {
        throw std::runtime_error("Pas possible d'extraire nom de fichier.");
    }
   
    
    // Détection des arrivées et du départ de la carte
    detection(&map);

    std::cout << "Depart x : " << map.Origine_x << "   Depart y : " << map.Origine_y << std::endl;

    for (int i = 0; i < map.nbr_arrive; i++)
    {
        std::cout << "Position : " << i << " x =" << map.Finish_x[i] << " | y =" << map.Finish_y[i] << std::endl;
    }
    pso.nbr_thread = pso.nbr_particule * map.nbr_arrive;

    std::cout << "------Ittération principale\n";
    particleStruct endPart = Algo_PSO(&map, &pso);
    output.sequence = endPart.Output;

    if (endPart.success != Base_atteinte) {
        std::cout << "------Ittération bis\n";
        
        map.matrice_mobile = endPart.matrice_mobile;
        map.Finish_x = {endPart.Finish_x};
        map.Finish_y = {endPart.Finish_y};
        map.nbr_arrive = 1;

        pso.nbr_particule = nbr_particule;
        pso.nbr_thread = nbr_particule;
        pso.omega = 1;

        particleStruct finalPart = Algo_PSO_bis(&map, &pso, &endPart);
        output.sequence += finalPart.Output;
        
    }

    // Paramètres dessin SVG
    svgStruct svg;
    svg.trajSuccess = 0;
    svg.output_file = "./Output/test.svg";
    svg.svgHeight = 40;

    // getPositionsOfSequence(&map,&svg,&output);
    // for(int i = 0;i<svg.trajX.size();i++) {
    //     std::cout << svg.trajX[i] << ":" << svg.trajY[i] << std::endl;
    // }
    // drawSVG(&map,&svg);

    create_ltr_file(&output);
    end = std::chrono::system_clock::now();
    long long int microseconde = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Temps d'execution du code " << microseconde << " microsec" << std::endl
              << std::endl;

    return 0;
}