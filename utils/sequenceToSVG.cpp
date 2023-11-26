#include "../Libs/sequenceToSVG.h"

void drawSVG (int matrice[16][16] ,int* posOriginX, int* posOriginY, int* posFinishX, int* posFinishY, int* nbr_arrive, std::string sequence, std::string output_file) {
// Taille du SVG - A adapter à votre écran
    int svgHeight = 40;

    // Création fichier
    std::ofstream fichier(output_file);
    int height = sizeof(matrice)/sizeof(matrice[0]);
    int width = sizeof(matrice[0])/sizeof(matrice[0][0]);
    if (fichier.is_open()) {

        // En-tête SVG
        fichier << "<svg version='1.2' xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' aria-labelledby='title' role='img'>'\n";

        // Matrice
        fichier << "\t<g>\n\t\t<rect x='0' y='0' width='" << (width+1)*svgHeight << "' height='" << (height+1)*svgHeight << "' style='fill:none;stroke:black;stroke-width:5'/>\n\t</g>\n\t<g>\n"; 
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                fichier << "\t\t<circle cx='" << (i+1)*svgHeight << "' cy='" << (j+1)*svgHeight << "' r='2'/>\n";
            }
        }
        fichier << "\t</g>\n";

        // Départ & arrivées
        fichier << "\t<g>\n\t\t<circle cx='" << (*posOriginX+1)*svgHeight << "' cy='" << (*posOriginY+1)*svgHeight << "' r='5' fill='none' stroke='black'/>\n\t</g>\n";
        for (int i = 0; i < *nbr_arrive; i++) {
            fichier << "\t<g>\n\t\t<circle cx='" << (posFinishX[i]+1)*svgHeight << "' cy='" << (posFinishY[i]+1)*svgHeight << "' r='5' fill='red'/>\n";
        }
        fichier << "\t</g>\n</svg>";

        fichier.close();
        std::cout << "Le fichier " << " a ete cree avec succes." << std::endl;
    }
    else {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
    }

    // Dessin séquence
}

int main(int argc, char const *argv[])
{
    //Paramètres Parsage
    std::string nom_fichier = "./Maps/Beginner-I.lt4";
    std::string output_file = "./Output/sequence.svg";
    std::string sequence = "UUUUUUUUUUUUUUUUU";
    int matrice[16][16];
    int nbr_arrive = 0;
    //Paramètres Deplacement
    int Origine_x;
    int Origine_y;
    int Finish_x[5] = { 0,0,0,0,0 };
    int Finish_y[5] = { 0,0,0,0,0 };

    // Import matrice
    parsage(nom_fichier,matrice);

    // Détection et dessin arrivées/Départ
    detection(matrice, &Origine_x, &Origine_y, Finish_x, Finish_y, &nbr_arrive);

    // Etablissement du trajet
    int seqLength = sequence.length();
    int trajX[seqLength],trajY[seqLength];
    

    drawSVG(matrice, &Origine_x, &Origine_y, Finish_x, Finish_y, &nbr_arrive, sequence,output_file);
}
