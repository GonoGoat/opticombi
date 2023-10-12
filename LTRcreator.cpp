#include <iostream>
#include <fstream>
#include <string>

void create_ltr_file(const std::string& name, const std::string& solver, const std::string& sequence, const std::string& output_file) {
    
    int score = sequence.length();

    // Ouvrir le fichier en mode écriture
    std::ofstream fichier(output_file);
    if (fichier.is_open()) {
        fichier << "Name: " << name << "\n";
        fichier << "Solver: " << solver << "\n";
        fichier << "Score: " << score << "\n";
        fichier << "\n" << sequence << "\n";
        fichier.close();
        std::cout << "Le fichier " << output_file << " a été créé avec succès." << std::endl;
    } else {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
    }
}

int main() {
    std::string name = "One block into the water";
    std::string solver = "Yono-Yono";
    std::string sequence = "RRUFFUULLUUURRDFDLLDDRFFFRRRDDDRRRUUULFDDDLLLUFFFUUUURRUUURRUUULLLDDLLDDDRFUUURRUURRRDDDLLDFDLLDDRFFFRRRDDRRRRRUUULLLLDFDLLDDRFFFRUUURRRRDFFDDDLLDDDRRRUULFFLLLL";
    std::string output_file = "mon_fichier.ltr";

    create_ltr_file(name, solver, sequence, output_file);

    return 0;
}