#include "sequenceToSVG.h"

/**
 * @brief Encapsule des éléments SVG dans une balise <g>...</g>
 * 
 * @param element IN - Element a encapsuler 
 * @return std::string Les éléments encapsulé
 */
std::string getSectionWithElement(std::string* element) {
    return ("\n<g>" + *element + "\n</g>");
}

/**
 * @brief Génère un cercle SVG
 * 
 * @param cx IN - Coordonnées X du centre
 * @param cy IN - Coordonnées Y du centre
 * @param r IN - Valeur du rayon
 * @param style IN - Style appliqué sur la forme au format SVG (<clé>:<valeur>;...)
 * @return std::string Le cercle aux propriétés désirées
 */
std::string getCircle(int cx, int cy, int r, std::string style) {
    return ("\n<circle cx='" + std::to_string(cx) + "' cy='" + std::to_string(cy) + "' r='" + std::to_string(r) + "' style='" + style + "'/>");
}

/**
 * @brief Génère un rectangle SVG
 * 
 * @param x IN - Coordonnées X du coin supérieur gauche
 * @param y IN - Coordonnées Y du coin supérieur gauche
 * @param width IN - Largeur du rectangle
 * @param height IN - Hauteur du rectangle
 * @param style IN - Style appliqué sur la forme au format SVG
 * @return std::string Le rectangle aux propriétés désirées
 */
std::string getRect(int x, int y, int width, int height, std::string style) {
    return ("\n<rect x='" + std::to_string(x) + "' y='" + std::to_string(y) + "' width='" + std::to_string(width) + "' height='" + std::to_string(height) + "' style='" + style + "'/>");
}

/**
 * @brief Génère une ligne SVG
 * 
 * @param x1 IN - Coordonnées X du premier point de la ligne
 * @param y1 IN - Coordonnées Y du premier point de la ligne
 * @param x2 IN - Coordonnées X du deuxième point de la ligne
 * @param y2 IN - Coordonnées Y du deuxième point de la ligne
 * @param style IN - Style appliqué sur la forme au format SVG
 * @return std::string La ligne aux propriétés désirées
 */
std::string getLine (int x1, int y1, int x2, int y2, std::string style) {
    return ("\n<line x1='" + std::to_string(x1) + "' y1='" + std::to_string(y1) + "' x2='" + std::to_string(x2) + "' y2='" + std::to_string(y2) +"' style='" + style + "'/>");
}

void drawSVG (mapStruct* mapParams, svgStruct* svgParams) {

    // Création fichier
    std::ofstream fichier(svgParams->output_file);
    if (fichier.is_open()) {

        // En-tête SVG
        fichier << "<svg version='1.2' xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' aria-labelledby='title' role='img'>'";

        // Contour matrice
        std::string matrixElement = getRect(0,0,((mapParams->nbr_colonnes+1)*svgParams->svgHeight),((mapParams->nbr_lignes+1)*svgParams->svgHeight),"fill:none;stroke:black;stroke-width:5");
        fichier << getSectionWithElement(&matrixElement);

        // Points matrice
        std::string dotsElement = "";
        for (int i = 0; i < mapParams->nbr_lignes; i++) {
            for (int j = 0; j < mapParams->nbr_colonnes; j++) {
                dotsElement += getCircle(((j+1)*svgParams->svgHeight), ((i+1)*svgParams->svgHeight), 2, "");
            }
        }
        fichier << getSectionWithElement(&dotsElement);

        // Départ de la carte "fill:none;stroke:black"
        std::string mapStartElement = getCircle(((mapParams->Origine_x+1)*svgParams->svgHeight),((mapParams->Origine_y+1)*svgParams->svgHeight), 5, "fill:green");
        fichier << getSectionWithElement(&mapStartElement);

        // Arrivées de la carte
        std::string mapEndsElement = "";
        for (int i = 0; i < mapParams->nbr_arrive; i++) {
            mapEndsElement += getCircle(((mapParams->Finish_x[i]+1)*svgParams->svgHeight),((mapParams->Finish_y[i]+1)*svgParams->svgHeight),5, "fill:red");
        }
        fichier << getSectionWithElement(&mapEndsElement);

        // Départ de la particule
        // TODO : A adapter à position de début
        std::string partStartElement = getCircle(((mapParams->Origine_x+1)*svgParams->svgHeight),((mapParams->Origine_y+1)*svgParams->svgHeight), 5, "fill:none;stroke-width:2;stroke:black");
        fichier << getSectionWithElement(&partStartElement);

        // Trajectoire
        std::string trajElement = "";
        for (int i = 0; i < svgParams->trajX.size()-1; i++) {
            trajElement += getLine(((svgParams->trajX[i]+1)*svgParams->svgHeight),((svgParams->trajY[i]+1)*svgParams->svgHeight),((svgParams->trajX[i+1]+1)*svgParams->svgHeight), ((svgParams->trajY[i+1]+1)*svgParams->svgHeight),"stroke:black");
        }
        fichier << getSectionWithElement(&trajElement);

        fichier << "\n</svg>";

        fichier.close();
        std::cout << "Le fichier " << svgParams->output_file <<  " a ete cree avec succes." << std::endl;
    }
    else {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
    }
}