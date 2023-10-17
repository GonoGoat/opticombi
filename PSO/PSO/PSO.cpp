#include "Parseur.h"
#include "LogiqueDeplacement.h"
#include "LTRcreator.h"
#include "Detection_Start_End.h"

int main()
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    //Paramètres Parsage
    std::string nom_fichier = "Beginner-I.lt4";
    std::string matrice[16][16];
    //Paramètres Deplacement
    int Origine_x;
    int Origine_y;
    int Finish_x;
    int Finish_y;
    char Direction_tank = 'U';
    //Paramètres LTR
    std::string name = "One block into the water";
    std::string solver = "PSO";
    std::string sequence;
    std::string output_file = "Test_3.ltr";

    parsage("..\\..\\"+nom_fichier,matrice);
    detection(matrice, &Origine_x, &Origine_y, &Finish_x, &Finish_y);
    std::cout << "Depart x : " << Origine_x << "   Depart y : " << Origine_y << std::endl;
    std::cout << "Arrive x : " << Finish_x << "  Arrive y : " << Finish_y << std::endl;
    Deplacement(Origine_x, Origine_y, Finish_x, Finish_y, Direction_tank, &sequence);
    create_ltr_file(name, solver, sequence, output_file);
    end = std::chrono::system_clock::now();
    long long int microseconde = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Temp d'execution du code " << microseconde << " microsec" << std::endl << std::endl;
}
