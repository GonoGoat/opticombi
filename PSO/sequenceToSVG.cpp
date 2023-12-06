#include "sequenceToSVG.h"

std::string getSectionWithElement(std::string element) {
    return ("\n<g>" + element + "\n</g>");
}

std::string getCircle(int cx, int cy, int r, std::string style) {
    return ("\n<circle cx='" + std::to_string(cx) + "' cy='" + std::to_string(cy) + "' r='" + std::to_string(r) + "' style='" + style + "'/>");
}

std::string getRect(int x, int y, int width, int height, std::string style) {
    return ("\n<rect x='" + std::to_string(x) + "' y='" + std::to_string(y) + "' width='" + std::to_string(width) + "' height='" + std::to_string(height) + "' style='" + style + "'/>");
}

std::string getLine (int x1, int y1, int x2, int y2, std::string style) {
    return ("\n<line x1='" + std::to_string(x1) + "' y1='" + std::to_string(y1) + "' x2='" + std::to_string(x2) + "' y2='" + std::to_string(y2) +"' style='" + style + "'/>");
}

void drawSVG (std::vector<std::vector<int>> matrice, int Origine_x, int Origine_y, std::vector<int> Finish_x, std::vector<int> Finish_y, int nbr_arrive, std::string sequence,std::string output_file, int nombreLignes, int nombreColonnes, std::vector<int> trajX, std::vector<int> trajY, int succes) {
    // Taille du SVG - A adapter à votre écran
    int svgHeight = 30;

    // Création fichier
    std::ofstream fichier(output_file);
    if (fichier.is_open()) {

        // En-tête SVG
        fichier << "<svg version='1.2' xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' aria-labelledby='title' role='img'>'";

        // Contour matrice
        std::string matrixElement = getRect(0,0,((nombreColonnes+1)*svgHeight),((nombreLignes+1)*svgHeight),"fill:none;stroke:black;stroke-width:5");
        fichier << getSectionWithElement(matrixElement);

        // Points matrice
        std::string dotsElement = "";
        for (int i = 0; i < nombreLignes; i++) {
            for (int j = 0; j < nombreColonnes; j++) {
                dotsElement += getCircle(((j+1)*svgHeight), ((i+1)*svgHeight), 2, "");
            }
        }
        fichier << getSectionWithElement(dotsElement);

        // Départ de la carte "fill:none;stroke:black"
        std::string mapStartElement = getCircle(((Origine_x+1)*svgHeight),((Origine_y+1)*svgHeight), 5, "fill:green");
        fichier << getSectionWithElement(mapStartElement);

        // Arrivées de la carte
        std::string mapEndsElement = "";
        for (int i = 0; i < nbr_arrive; i++) {
            mapEndsElement += getCircle(((Finish_x[i]+1)*svgHeight),((Finish_y[i]+1)*svgHeight),5, "fill:red");
        }
        fichier << getSectionWithElement(mapEndsElement);

        // Trajectoire
        std::string trajElement = "";
        for (int i = 0; i < trajX.size()-1; i++) {
            trajElement += getLine(((trajX[i]+1)*svgHeight),((trajY[i]+1)*svgHeight),((trajX[i+1]+1)*svgHeight), ((trajY[i+1]+1)*svgHeight),"stroke:black");
        }
        fichier << getSectionWithElement(trajElement);

        fichier << "\n</svg>";

        fichier.close();
        //std::cout << "Le fichier " << " a ete cree avec succes." << std::endl;
    }
    else {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
    }

    // Dessin séquence
}