#include "Parseur.h"
#include "LogiqueDeplacement.h"
#include "LTRcreator.h"

int main()
{
    //Paramètres Parsage
    std::string nom_fichier = "Beginner-II.lt4";
    //Paramètres Deplacement
    int Origine_x = 4;
    int Origine_y = 4;
    int Finish_x = 1;
    int Finish_y = 8;
    char Direction_tank = 'U';
    //Paramètres LTR
    std::string name = "One block into the water";
    std::string solver = "PSO";
    std::string sequence;
    std::string output_file = "Test.ltr";

    parsage("..\\..\\"+nom_fichier);
    Deplacement(Origine_x, Origine_y, Finish_x, Finish_y, Direction_tank, &sequence);
    create_ltr_file(name, solver, sequence, output_file);
}
