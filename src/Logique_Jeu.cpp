#include "Logique_jeu.h"
/**
 * @brief Liste toutes les positions d'un tank lors de l'exécution d'une séquence
 * 
 * @param matrice IN - La matrice parsee
 * @param sequence IN - La séquence jouée par le tank dans la matrice
 * @param trajX IN/OUT - L'historique des positions en X du tank
 * @param trajY IN/OUT - L'historique des positions en Y du tank
 * @param trajSuccess IN/OUT - L'etat du tank après l'exécution de la séquence
 */
void getPositionsOfSequence(std::vector<std::vector<int>>* matrice, std::string& sequence, std::vector<int>* trajX, std::vector<int>* trajY, int* trajSuccess) {
    // std::cout << (*trajX)[0];
    // Etablir les positions X et Y de départ
    int posX = (*trajX)[0]; 
    int posY = (*trajY)[0];

    // Initialisation des variables de lancement du moteur
    int success;
    char dir;
    
    // Jusque dernier caractère de séquence
    for(int i = 0;i<sequence.size();i++) {

        // Extraction de la séquence à jouer
        std::string subSeq = sequence.substr(0,i+1);

        // Remise à zéro
        dir = 'U';
        success = 0;

        // Préparer les positions
        (*trajX).push_back(posX);
        (*trajY).push_back(posY);

        // Jeu de la séqeuence
        Engine(matrice, subSeq, &(*trajX)[i+1], &(*trajY)[i+1], &dir, &success);
    }
    *trajSuccess = success;
}

/**
 * @brief Execute la logique du jeu
 * 
 * @param matrice IN - La carte parsée
 * @param sequence IN - La séquence jouée
 * @param posX IN/OUT - La position initiale (IN) et finale (OUT) du tank en X après exécution de la séquence
 * @param posY IN/OUT - La position initiale (IN) et finale (OUT) du tank en Y après exécution de la séquence
 * @param dir_previous IN/OUT - L'orientation initiale (IN) et finale (OUT) du tank après exécution de la séquence
 * @param succes OUT - Représentation de l'état du tank après exécution de la séquence : 
 */
void Engine(std::vector<std::vector<int>>* matrice, const std::string& sequence, int* posX, int* posY, char *dir_previous, int* succes) {
    //Variables pour traiter le d�placement sans le ressortir directement
    int deplacement_x, deplacement_y;

    //Traite la s�quence d'entr�e
    for (char dir_actuelle : sequence)
    {
        //Logique de fonctionnement du jeu : 2 direction identiques pour effectuer le d�placement
        if (dir_actuelle == *dir_previous) {

            deplacement_x = *posX;
            deplacement_y = *posY;

            Deplacement(&dir_actuelle, &deplacement_x, &deplacement_y);
            Verification_deplacement(matrice, &deplacement_x, &deplacement_y, posX, posY, succes);
            std::cout << "deplacement x : " << *posX << " |deplacement y : " << *posY << std::endl;
            *succes = 0;
        }
        //Tir g�rer s�paremment car une seul instance suffit pour tirer
        else if (dir_actuelle == 'F') {
            Tir();
        }
        //Enregistre les changement de direction du tank sans le faire bouger sur la carte
        else {
            *dir_previous = dir_actuelle;
        }
    }
}

//Fonction permettant de calculer le d�placement � effectuer sur la particule sur base du caract�re de la s�quence -> conversion
/**
 * @brief Calcul du déplacement à effectuer sur le tank sur base d'un caractère
 * 
 * @param dir IN - Direction demandée
 * @param pos_x OUT - Position en X modifiée par la direction demandée
 * @param pos_y OUT - Position en Y modifiée par la direction demandée
 */
void Deplacement(char* dir, int* pos_x, int* pos_y) {
    switch (*dir) {
    case 'U':
        *pos_y -= 1;
        break;
    case 'D':
        *pos_y += 1;
        break;
    case 'R':
        *pos_x += 1;
        break;
    case 'L':
        *pos_x -= 1;
        break;
    }
}

//Fonction de verification de la possibilit� d'effectuer le d�placement rentr�
void Verification_deplacement(std::vector<std::vector<int>>* matrice, int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes){

    //V�rification des limites si d�passe pas de mouvement enregistr�
    if (*depl_x > 15) {
        *depl_x = 15;
    }
    else if (*depl_x < 0) {
        *depl_x = 0;
    }
    else if (*depl_y > 15) {
        *depl_y = 15;
    }
    else if (*depl_y < 0) {
        *depl_y = 0;
    }
    //V�rifie une position valide
    else {
        switch ((*matrice)[*depl_y][*depl_x])
        {
        case Dirt :
        case Bridge:
            Verification_Anti_Tank(matrice, depl_x, depl_y, pos_x, pos_y, succes);
            break;
            /**pos_x = *depl_x;
            *pos_y = *depl_y;*/
        case Base :
            /**pos_x = *depl_x;
            *pos_y = *depl_y;*/
            *succes = 1;
            std::cout << "Base atteinte" << std::endl;

            Verification_Anti_Tank(matrice, depl_x, depl_y, pos_x, pos_y, succes);
            break;
        case Water :
            *succes = -1;
            std::cout << "Mort dans l'eau a la case (" << *depl_x << "," << *depl_y << ")" << std::endl;
            break;
        case Way_U :
        case Way_D :
        case Way_R :
        case Way_L :
            break;
        case Ice :
            break;
        case Thin_Ice :
            break;
        case Tunnel_Red :
        case Tunnel_Green :
        case Tunnel_Blue :
        case Tunnel_Cyan : 
        case Tunnel_Yellow :
        case Tunnel_Pink :
        case Tunnel_White :
        case Tunnel_Dark :
            Portail(matrice, (*matrice)[*depl_y][*depl_x], depl_x, depl_y);
            //Verification anti tank du c�t� du portail de sortie
            Verification_Anti_Tank(matrice, depl_x, depl_y, pos_x, pos_y, succes);
            break;
        default:
            break;
        }
    }

}

//Fonction qui traite le passage au travers un portail afin de sortir le tank au bon endroit
void Portail(std::vector<std::vector<int>>* matrice, int couleur, int* depl_x, int* depl_y)
{
    bool sortie = false;

    //Parse la carte pour trouver le premier portail correspondant au portail d'entr�e mais �tant diff�rant.
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            if ((*matrice)[i][j] == couleur && (i != *depl_y || j != *depl_x)) {
                *depl_y = i;
                *depl_x = j;
                //sortie du parseur quand trouv� le premier portail correspondant
                sortie = true;
                break;
            }
        }
        //sortie du parseur quand trouv� le premier portail correspondant
        if (sortie) {
            break;
        }
    }
}

//Fonction permettant de v�rifier si un anti tank ne nous tue pas en effectuant le d�placement
void Verification_Anti_Tank(std::vector<std::vector<int>>* matrice, int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes)
{
    int position = (*matrice)[*depl_y][*depl_x];
    //Verification horizontale
        //Au dessus de la position en cours
    for (int i = 0; i < *depl_y; i++) {
        position = (*matrice)[*depl_y - i][*depl_x];
        if ((position >=Sollid_Block && position <= Anti_Tank_U) || (position>= Anti_Tank_R && position <= Mirro_DL)||(position>= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << position << std::endl;
            break;
        }
        else if (position == Anti_Tank_D) {
            *succes = -1;
        }
    }
        //En dessous de la position en cours
    for (int i = 0; i < 15 - *depl_y; i++) {
        position = (*matrice)[*depl_y + i][*depl_x];
        if ((position >= Sollid_Block && position <= Bricks) || (position >= Anti_Tank_D && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_U) {
            *succes = -1;
        }
    }

    //Verification verticale
        //A gauche de la position en cours
    for (int i = 0; i < *depl_x; i++) {
        position = (*matrice)[*depl_y][*depl_x - i];
        if ((position >= Sollid_Block && position <= Anti_Tank_D) || (position >= Anti_Tank_L && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_R) {
            *succes = -1;
        }
    }
        //A droite de la position en cours
    for (int i = 0; i < 15 - *depl_y; i++) {
        position = (*matrice)[*depl_y][*depl_x + i];
        if ((position >= Sollid_Block && position <= Anti_Tank_R) || (position >= Mirror_UR && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_L) {
            *succes = -1;
        }
    }

    //Si encore en vie alors deplacement effectu�
    if (*succes != -1) {
        std::cout << "Deplacement OK" << std::endl;
        *pos_x = *depl_x;
        *pos_y = *depl_y;
    }
}

void Tir()
{
}