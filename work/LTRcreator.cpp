#include "LTRcreator.h"

/**
 * @brief Génère un fichier lt4
 * 
 * @param outputParams Les paramètres du fichier lt4
 */
void create_ltr_file(outputStruct* outputParams) {

    int score = outputParams->sequence.length();

    // Ouvrir le fichier en mode écriture
    std::ofstream fichier(outputParams->output_file);
    if (fichier.is_open()) {
        fichier << "Name: " << outputParams->name << "\n";
        fichier << "Solver: " << outputParams->solver << "\n";
        fichier << "Score: " << score << "\n";
        fichier << "\n" << outputParams->sequence << "\n";
        fichier.close();
        std::cout << "Le fichier " << outputParams->output_file << " a ete cree avec succes." << std::endl;
    }
    else {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
    }
}