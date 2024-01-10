#include "IdentifierObstacle.h"

bool IdentifierObstacle(particleStruct *partParams, double influence,mapStruct* mapParams)
{
    /*
    std::cout << "La direction : " << partParams->Direction_tank << std::endl;
    std::cout << "X : " << partParams->posX;
    std::cout << " Y : " << partParams->posY << std::endl;
    */
    switch (partParams->Direction_tank)
    {
    case 'U':
        // Parcours le reste de la ligne ou colonne selon sa direction
        for (int i = partParams->posY; i > 0; i--)
        {
            // Bloc solide detecté
            if (mapParams->matrice_fixe[i][partParams->posX] == Sollid_Block)
            {
                return false;
            }
            else if (partParams->matrice_mobile[i][partParams->posX] != 0)
            {
                // std::cout << "Obstacle trouvé sur la ligne " << partParams->posX << ", colonne " << i << std::endl;
                // std::cout << partParams->matrice_mobile[i][partParams->posX] << std::endl;
                // Bloc détecté
                if (faireChoix(influence))
                {
                    // Choix de tirer
                    // std::cout << partParams->matrice_mobile[i][partParams->posX] << std::endl;
                    // std::cout << "Obstacle trouvé sur la ligne " << partParams->posX << ", colonne " << i << std::endl;
                    return true;
                }
                else
                    return false;
            }
            // std::cout << "laa Aucun obstacle trouvé dans cette direction." << std::endl;
        }
        break;
    case 'D':
        for (int i = partParams->posY; i < partParams->matrice_mobile[0].size(); i++)
        {
            // Bloc solide detecté
            if (mapParams->matrice_fixe[i][partParams->posX] == Sollid_Block)
            {
                return false;
            }
            else if (partParams->matrice_mobile[i][partParams->posX] != 0)
            {
                // std::cout << "Obstacle trouvé sur la ligne " << partParams->posX << ", colonne " << i << std::endl;
                // std::cout << partParams->matrice_mobile[i][partParams->posX] << std::endl;
                if (faireChoix(influence))
                {
                    // Choix de tirer
                    // std::cout << partParams->matrice_mobile[i][partParams->posX] << std::endl;
                    // std::cout << "Obstacle trouvé sur la ligne " << partParams->posX << ", colonne " << i << std::endl;
                    return true;
                }
                else
                    return false;
            }
            // std::cout << "Aucun obstacle trouvé dans cette direction." << std::endl;
        }
        break;
    case 'L':
        for (int i = partParams->posX; i > 0; i--)
        {
            if (mapParams->matrice_fixe[partParams->posY][i] == Sollid_Block)
            {
                return false;
            }
            else if (partParams->matrice_mobile[partParams->posY][i] != 0)
            {
                // std::cout << "Obstacle trouvé sur la colonne " << partParams->posY << ", ligne " << i << std::endl;
                // std::cout << partParams->matrice_mobile[partParams->posX][i] << std::endl;
                if (faireChoix(influence))
                {
                    // Choix de tirer
                    // std::cout << partParams->matrice_mobile[partParams->posY][partParams->posY] << std::endl;
                    // std::cout << "Obstacle trouvé sur la ligne " << i << ", colonne " << partParams->posY << std::endl;
                    return true;
                }
                else
                    return false;
            }
            // std::cout << "Aucun obstacle trouvé dans cette direction." << std::endl;
        }

        break;
    case 'R':
        for (int i = partParams->posX; i < partParams->matrice_mobile.size(); i++)
        {
            if (mapParams->matrice_fixe[partParams->posY][i] == Sollid_Block)
            {
                return false;
            }
            else if (partParams->matrice_mobile[partParams->posY][i] != 0)
            {
                // std::cout << "Obstacle trouvé sur la colonne " << partParams->posY << ", ligne " << i << std::endl;
                // std::cout << partParams->matrice_mobile[partParams->posX][i] << std::endl;
                if (faireChoix(influence))
                {
                    // Choix de tirer
                    // std::cout << partParams->matrice_mobile[partParams->posY][i] << std::endl;
                    // std::cout << "Obstacle trouvé sur la ligne " << i << ", colonne " << partParams->posY << std::endl;

                    return true;
                }
                else
                    return false;
            }
            // std::cout << "Aucun obstacle trouvé dans cette direction." << std::endl;
        }
        break;
    default:
        // std::cout << "pas de direction correct" << std::endl;
        break;
    }
    return false;
};

bool faireChoix(double influence)
{
    // Génération d'un nombre aléatoire entre 0 et 1
    double randomValue = (double)rand() / RAND_MAX;
    // std::cout << randomValue << std::endl;
    //  Condition pour choisir en fonction de l'influence
    if (randomValue < influence)
    {
        // Choix de tirer
        return true;
    }
    else
    {
        // Choix de se déplacer
        return false;
    }
}