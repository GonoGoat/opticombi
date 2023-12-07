#include "Logique_Jeu.h"

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
void Engine(std::vector<std::vector<int>>* matrice, std::vector<std::vector<int>>* matrice_mobile, const std::string& sequence, int* posX, int* posY, char* dir_previous, int* succes) {
    //Variables pour traiter le déplacement sans le ressortir directement
    int deplacement_x, deplacement_y;
    //int taille_sequence = 0;

    //Traite la séquence d'entrée
    for (char dir_actuelle : sequence)
    {
        //Logique de fonctionnement du jeu : 2 direction identiques pour effectuer le déplacement
        if (dir_actuelle == *dir_previous) {

            deplacement_x = *posX;
            deplacement_y = *posY;

            Deplacement(&dir_actuelle, &deplacement_x, &deplacement_y);
            Verification_deplacement(matrice, matrice_mobile, &deplacement_x, &deplacement_y, posX, posY, succes, &dir_actuelle);
            std::cout << "deplacement x : " << *posX << " |deplacement y : " << *posY << std::endl;
            *succes = 0;
        }
        //Tir gérer séparemment car une seul instance suffit pour tirer
        else if (dir_actuelle == 'F') {
            Tir(matrice,matrice_mobile, *posX, *posY, *dir_previous, succes);
            *succes = 0;
        }
        //Enregistre les changement de direction du tank sans le faire bouger sur la carte
        else {
            *dir_previous = dir_actuelle;
        }
        //taille_sequence++;
    }
}

//Fonction permettant de calculer le déplacement à effectuer sur la particule sur base du caractére de la séquence -> conversion
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

//Fonction de verification de la possibilité d'effectuer le déplacement rentré
void Verification_deplacement(std::vector<std::vector<int>>* matrice, std::vector<std::vector<int>>* matrice_mobile, int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes, char* dir) {

    //Vérification des limites si dépasse pas de mouvement enregistré
    if (*depl_x > 15) {
        std::cout << "Depassement limite" << std::endl;
        *depl_x = 15;
    }
    else if (*depl_x < 0) {
        *depl_x = 0;
    }
    else if (*depl_y > 15) {
        std::cout << "Depassement limite" << std::endl;
        *depl_y = 15;
    }
    else if (*depl_y < 0) {
        *depl_y = 0;
    }
    //Vérifie une position valide
    else {
        switch ((*matrice)[*depl_y][*depl_x] + (*matrice_mobile)[*depl_y][*depl_x])
        {
        case Dirt:
        case Bridge:
        case Tank :
            Verification_Anti_Tank(matrice, matrice_mobile, depl_x, depl_y, pos_x, pos_y, succes);
            break;
            /**pos_x = *depl_x;
            *pos_y = *depl_y;*/
        case Base:
            /**pos_x = *depl_x;
            *pos_y = *depl_y;*/
            *succes = 1;
            std::cout << "Base atteinte" << std::endl;

            Verification_Anti_Tank(matrice, matrice_mobile, depl_x, depl_y, pos_x, pos_y, succes);
            break;
        case Water:
            *succes = -1;
            std::cout << "Mort dans l'eau a la case (" << *depl_x << "," << *depl_y << ")" << std::endl;
            break;
        case Way_U:
        case Way_D:
        case Way_R:
        case Way_L:
            path(dir, matrice, matrice_mobile, depl_x, depl_y, pos_x, pos_y, succes);
            break;
        case Ice:
            glace(dir, matrice, matrice_mobile,depl_x,depl_y, pos_x, pos_y, succes);
            break;
        case Thin_Ice:
            glace_fine(dir, matrice, matrice_mobile, depl_x, depl_y, pos_x, pos_y, succes);
            break;
        case Tunnel_Red:
        case Tunnel_Green:
        case Tunnel_Blue:
        case Tunnel_Cyan:
        case Tunnel_Yellow:
        case Tunnel_Pink:
        case Tunnel_White:
        case Tunnel_Dark:
            Portail(matrice, matrice_mobile, (*matrice)[*depl_y][*depl_x], depl_x, depl_y,succes);
            //Verification anti tank du côté du portail de sortie
            Verification_Anti_Tank(matrice, matrice_mobile, depl_x, depl_y, pos_x, pos_y, succes);
            break;
        default:
            break;
        }
    }

}

//Fonction qui traite le passage au travers un portail afin de sortir le tank au bon endroit
void Portail(std::vector<std::vector<int>>* matrice, std::vector<std::vector<int>>* matrice_mobile, int couleur, int* depl_x, int* depl_y, int* succes)
{
    bool sortie = false;

    //Parse la carte pour trouver le premier portail correspondant au portail d'entrée mais étant différant.
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            if ((*matrice)[i][j] == couleur && (i != *depl_y || j != *depl_x) && ((* matrice_mobile)[i][j] == Dirt)) {
                *depl_y = i;
                *depl_x = j;
                std::cout << "Sortie au portail situe en " << i << ", " << j << std::endl;
                //sortie du parseur quand trouvé le premier portail correspondant
                sortie = true;
                break;
            }
        }
        //sortie du parseur quand trouvé le premier portail correspondant
        if (sortie) {
            break;
        }
    }
    //Si pas sorti par un tunnel alors mort.
    if (sortie == false) {
        *succes = -1;
        std::cout << "Mort car pas de sortie au portail" << std::endl;
    }
}

//Fonction permettant de vérifier si un anti tank ne nous tue pas en effectuant le déplacement
void Verification_Anti_Tank(std::vector<std::vector<int>>* matrice, std::vector<std::vector<int>>* matrice_mobile, int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes)
{
    //!\ ICI utilisation d'une méthode afin de ne pas devoir repenser les conditions car la somme des deux donnera le bon élément (1 des 2 toujours = 0)
    int position = (*matrice)[*depl_y][*depl_x] + (*matrice_mobile)[*depl_y][*depl_x];

    //Verification verticale
        //A droite de la position en cours
    for (int i = 1; i < 16 - *depl_x; i++) {
        position = (*matrice)[*depl_y][*depl_x + i] + (*matrice_mobile)[*depl_y][*depl_x + i];
        if ((position >= Sollid_Block && position <= Anti_Tank_R) || (position >= Mirror_UR && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_L) {
            *succes = -1;
        }
    }
        //A gauche de la position en cours
    for (int i = 1; i < *depl_x + 1; i++) {
        position = (*matrice)[*depl_y][*depl_x - i] + (*matrice_mobile)[*depl_y][*depl_x - i];
        if ((position >= Sollid_Block && position <= Anti_Tank_D) || (position >= Anti_Tank_L && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_R) {
            *succes = -1;
        }
    }

    //Verification vertical
        //En dessous de la position en cours
    for (int i = 1; i < 16 - *depl_y; i++) {
        position = (*matrice)[*depl_y + i][*depl_x] + (*matrice_mobile)[*depl_y + i][*depl_x];
        if ((position >= Sollid_Block && position <= Bricks) || (position >= Anti_Tank_D && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_U) {
            *succes = -1;
        }
    }
        //Au dessus de la position en cours
    for (int i = 1; i < *depl_y + 1; i++) {
        position = (*matrice)[*depl_y - i][*depl_x] + (*matrice_mobile)[*depl_y - i][*depl_x];
        if ((position >= Sollid_Block && position <= Anti_Tank_U) || (position >= Anti_Tank_R && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << position << std::endl;
            break;
        }
        else if (position == Anti_Tank_D) {
            *succes = -1;
        }
    }

    //Si encore en vie alors deplacement effectué
    if (*succes != -1) {
        std::cout << "Deplacement OK" << std::endl;
        *pos_x = *depl_x;
        *pos_y = *depl_y;
    }
}

//Fonction permettant de vérifier si un anti tank ne nous tue pas autour d'un parcour continue sur l'axe y (chemin/glace)
void Verification_Anti_Tank_parcour_vertical(std::vector<std::vector<int>>* matrice, std::vector<std::vector<int>>* matrice_mobile, int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes)
{
    std::cout << "Verification vertical" << std::endl;
    int position = (*matrice)[*depl_y][*depl_x] + (*matrice_mobile)[*depl_y][*depl_x];

    //Verification horizontal
        //A droite de la position en cours
    for (int i = *depl_x + 1; i < *depl_x + 2 && i < 15; i++) {
        position = (*matrice)[*depl_y][i] + (*matrice_mobile)[*depl_y][i];
        if ((position >= Sollid_Block && position <= Anti_Tank_R) || (position >= Mirror_UR && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_L) {
            std::cout << "Anti Tank Left vert" << std::endl;
            *succes = -1;
        }
    }
        //A gauche de la position en cours
    for (int i = *depl_x - 1; i < *depl_x && i > 0; i++) {
        position = (*matrice)[*depl_y][i] + (*matrice_mobile)[*depl_y][i];
        if ((position >= Sollid_Block && position <= Anti_Tank_D) || (position >= Anti_Tank_L && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_R) {
            std::cout << "Anti Tank Right vert" << std::endl;
            *succes = -1;
        }
    }

    //Verification vertical
        //En dessous de la position en cours
    for (int i = 1; i < 16 - *depl_y; i++) {
        position = (*matrice)[*depl_y + i][*depl_x] + (*matrice_mobile)[*depl_y + i][*depl_x];
        if ((position >= Sollid_Block && position <= Bricks) || (position >= Anti_Tank_D && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_U) {
            std::cout << "Anti Tank UP chemin vert" << std::endl;
            *succes = -1;
        }
    }
        //Au dessus de la position en cours
    for (int i = 1; i < *depl_y + 1; i++) {
        position = (*matrice)[*depl_y - i][*depl_x] + (*matrice_mobile)[*depl_y - i][*depl_x];
        if ((position >= Sollid_Block && position <= Anti_Tank_U) || (position >= Anti_Tank_R && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << position << std::endl;
            break;
        }
        else if (position == Anti_Tank_D) {
            std::cout << "Anti Tank Down chemin vert" << std::endl;
            *succes = -1;
        }
    }

}

//Fonction permettant de vérifier si un anti tank ne nous tue pas autour d'un parcour continue sur l'axe x (chemin/glace)
void Verification_Anti_Tank_parcour_horizontal(std::vector<std::vector<int>>* matrice, std::vector<std::vector<int>>* matrice_mobile, int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes)
{
    std::cout << "Verification horizontal" << std::endl;
    int position = (*matrice)[*depl_y][*depl_x] + (*matrice_mobile)[*depl_y][*depl_x];
    //Verification horizontal
        //A droite de la position en cours
    for (int i = 1; i < 16 - *depl_x; i++) {
        position = (*matrice)[*depl_y][*depl_x + i] + (*matrice_mobile)[*depl_y][*depl_x + i];
        if ((position >= Sollid_Block && position <= Anti_Tank_R) || (position >= Mirror_UR && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_L) {
            std::cout << "Anti Tank Left horiz" << std::endl;
            *succes = -1;
        }
    }
        //A gauche de la position en cours
    for (int i = 1; i < *depl_x + 1; i++) {
        position = (*matrice)[*depl_y][*depl_x - i] + (*matrice_mobile)[*depl_y][*depl_x - i];
        if ((position >= Sollid_Block && position <= Anti_Tank_D) || (position >= Anti_Tank_L && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_R) {
            std::cout << "Anti Tank Right horiz" << std::endl;
            *succes = -1;
        }
    }

    //Verification vertical
        //En dessous de la position en cours
    for (int i = *depl_y + 1; i < *depl_y + 2 && i < 15; i++) {
        position = (*matrice)[i][*depl_x] + (*matrice_mobile)[i][*depl_x];
        if ((position >= Sollid_Block && position <= Bricks) || (position >= Anti_Tank_D && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_U) {
            std::cout << "Anti Tank UP horiz" << std::endl;
            *succes = -1;
        }
    }
        //Au dessus de la position en cours
    for (int i = *depl_y - 1; i < *depl_y && i > 0; i++) {
        position = (*matrice)[i][*depl_x] + (*matrice_mobile)[i][*depl_x];
        if ((position >= Sollid_Block && position <= Anti_Tank_U) || (position >= Anti_Tank_R && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << position << std::endl;
            break;
        }
        else if (position == Anti_Tank_D) {
            std::cout << "Anti Tank Down horiz" << std::endl;
            *succes = -1;
        }
    }
}

//Fonction de calcul de trajectoire
void Tir(std::vector<std::vector<int>>* matrice, std::vector<std::vector<int>>* matrice_mobile, int depl_x, int depl_y, char dir_laser, int* succes)
{
    bool disparaitre = false;
    int pos_tank_x = depl_x;
    int pos_tank_y = depl_y;
    do
    {
        //Deplacement laser en fonction direction
        Deplacement(&dir_laser, &depl_x, &depl_y);

        //Vérification des limites si dépasse le laser disparait
        if (depl_x > 15 || depl_x < 0 || depl_y > 15 || depl_y < 0) {
            disparaitre = true;
        }
        else if (depl_x == pos_tank_x && depl_y == pos_tank_y) {
            *succes = -1;
            std::cout << "Auto Kill" << std::endl;
        }
        //Vérifie une position valide
        else {
            switch ((*matrice)[depl_y][depl_x] + (*matrice_mobile)[depl_y][depl_x])
            {
            case Mirror_DR:
            case Rotative_Mirror_DR:
                std::cout << "Miroir DR" << std::endl;
                if (dir_laser == 'U') {
                    dir_laser = 'R';
                }
                else if (dir_laser == 'L') {
                    dir_laser = 'D';
                }
                else {
                    std::cout << "Tir arrete" << std::endl;
                    disparaitre = true;
                }
                break;
            case Mirro_DL:
            case Rotative_Mirror_DL:
                std::cout << "Miroir DL" << std::endl;
                if (dir_laser == 'U') {
                    dir_laser = 'L';
                }
                else if (dir_laser == 'R') {
                    dir_laser = 'D';
                }
                else {
                    std::cout << "Tir arrete" << std::endl;
                    disparaitre = true;
                }
                break;
            case Mirror_UL:
            case Rotative_Mirror_UL:
                std::cout << "Miroir UL" << std::endl;
                if (dir_laser == 'D') {
                    dir_laser = 'L';
                }
                else if (dir_laser == 'R') {
                    dir_laser = 'U';
                }
                else {
                    std::cout << "Tir arrete" << std::endl;
                    disparaitre = true;
                }
                break;
            case Mirror_UR:
            case Rotative_Mirror_UR:
                std::cout << "Miroir UR" << std::endl;
                if (dir_laser == 'D') {
                    dir_laser = 'R';
                }
                else if (dir_laser == 'L') {
                    dir_laser = 'U';
                }
                else {
                    std::cout << "Tir arrete" << std::endl;
                    disparaitre = true;
                }
                break;
            case Bricks:
            case Sollid_Block:
                std::cout << "Tir arrete Sollid_Block ou Bricks" << std::endl;
                disparaitre = true;
                break;
            case Anti_Tank_U:
                if (dir_laser == 'D') {
                    std::cout << "Anti tank Up mort" << std::endl;
                }
                else {
                    std::cout << "Tir arrete Anti tank Up" << std::endl;
                }
                disparaitre = true;
                break;
            case Anti_Tank_D:
                if (dir_laser == 'U') {
                    std::cout << "Anti tank Down mort" << std::endl;
                }
                else {
                    std::cout << "Tir arrete Anti tank Down" << std::endl;
                }
                disparaitre = true;
                break;
            case Anti_Tank_L:
                if (dir_laser == 'R') {
                    std::cout << "Anti tank Left mort" << std::endl;
                }
                else {
                    std::cout << "Tir arrete Anti tank Left" << std::endl;
                }
                disparaitre = true;
                break;
            case Anti_Tank_R:
                if (dir_laser == 'L') {
                    std::cout << "Anti tank Right mort" << std::endl;
                }
                else {
                    std::cout << "Tir arrete Anti tank Right" << std::endl;
                }
                disparaitre = true;
                break;

            default:
                break;
            }
        }

    } while (disparaitre == false);
}

//Fonction pour l'interaction glace
void glace(char* direction, std::vector<std::vector<int>>* matrice, std::vector<std::vector<int>>* matrice_mobile, int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes) {

    std::cout << "Bonjour je suis la fonction Glace!Direction " << *direction << std::endl;
    int position;

    do {

        Deplacement(direction, depl_x, depl_y);

        position = (*matrice)[*depl_y][*depl_x] + (*matrice_mobile)[*depl_y][*depl_x];

        //Verifie si tank bloque si c'est le cas, il est juste arrêté et sa position est valide
        if (*depl_x < 0 || *depl_x>15 || *depl_y < 0 || *depl_y>15 || (position >= Sollid_Block && position <= Mirro_DL) || (position >= Crystal_Block && position <= Rotative_Mirror_DL)) {
            std::cout << "Position non valide" << std::endl;
            *succes = -2;
            break;
        }
        else if (*direction == 'U' || *direction == 'D') {
            Verification_Anti_Tank_parcour_vertical(matrice, matrice_mobile, depl_x, depl_y, pos_x, pos_y, succes);
        }
        else if (*direction == 'L' || *direction == 'R') {
            Verification_Anti_Tank_parcour_horizontal(matrice, matrice_mobile, depl_x, depl_y, pos_x, pos_y, succes);
        }
        std::cout << "Sur Glace deplacement x : " << *depl_x << " |deplacement y : " << *depl_y << std::endl;
    } while ((*matrice)[*depl_y][*depl_x] == Ice);

    if (*succes == 0) {
        Verification_deplacement(matrice, matrice_mobile, depl_x, depl_y, pos_x, pos_y, succes, direction);
    }
}


void glace_fine(char* direction, std::vector<std::vector<int>>* matrice, std::vector<std::vector<int>>* matrice_mobile, int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes) {
    
    std::cout << "Bonjour je suis la fonction Glace fine!Direction " << *direction << std::endl;
    int position;

    do {
        (*matrice)[*depl_y][*depl_x] = Water;

        Deplacement(direction, depl_x, depl_y);

        position = (*matrice)[*depl_y][*depl_x] + (*matrice_mobile)[*depl_y][*depl_x];

        //Verifie si le tank est bloqué dans son mouvement, si c'est le cas il est mort car la glace fine c'est transformé en eau et la séquence est invalide
        if (*depl_x < 0 || *depl_x>15 || *depl_y < 0 || *depl_y>15 || (position >= Sollid_Block && position <= Mirro_DL) || (position >= Crystal_Block && position <= Rotative_Mirror_DL)) {
            std::cout << "Position non valide, donc mort dans l'eau" << std::endl;
            *succes = -1;
            break;
        }
        else if (*direction == 'U' || *direction == 'D') {
            Verification_Anti_Tank_parcour_vertical(matrice, matrice_mobile, depl_x, depl_y, pos_x, pos_y, succes);
        }
        else if (*direction == 'L' || *direction == 'R') {
            Verification_Anti_Tank_parcour_horizontal(matrice, matrice_mobile, depl_x, depl_y, pos_x, pos_y, succes);
        }
        std::cout << "Sur Glace fine deplacement x : " << *depl_x << " |deplacement y : " << *depl_y << std::endl;
    } while ((*matrice)[*depl_y][*depl_x] == Thin_Ice);

    if (*succes != -1) {
        Verification_deplacement(matrice, matrice_mobile, depl_x, depl_y, pos_x, pos_y, succes, direction);
    }
}

void path(char* direction, std::vector<std::vector<int>>* matrice, std::vector<std::vector<int>>* matrice_mobile, int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes) {
    
    char dir_way;
    int position;

    std::cout << "Bonjour je suis la fonction Path!Direction " << *direction << std::endl;
    
    do {
        switch ((*matrice)[*depl_y][*depl_x])
        {
        case Way_U : 
            dir_way = 'U';
           break;
        case Way_D :
            dir_way = 'D';
            break;
        case Way_R :
            dir_way = 'R';
            break;
        case Way_L :
            dir_way = 'L';
            break;
        default:
            break;
        }

        Deplacement(&dir_way, depl_x, depl_y);

        position = (*matrice)[*depl_y][*depl_x] + (*matrice_mobile)[*depl_y][*depl_x];

        //Verifie si tank bloque si c'est le cas, il est juste arrêté et sa position est valide
        if (*depl_x < 0 || *depl_x>15 || *depl_y < 0 || *depl_y>15 || (position >= Sollid_Block && position <= Mirro_DL) || (position >= Crystal_Block && position <= Rotative_Mirror_DL)) {
            std::cout << "Position non valide" << std::endl;
            *succes = -2;
            switch (dir_way)
            {
            case 'U':
                dir_way = 'D';
                break;
            case 'D' :
                dir_way = 'U';
                break;
            case 'R':
                dir_way = 'L';
                break;
            case 'L' :
                dir_way = 'R';
                break;
            }
            Deplacement(&dir_way, depl_x, depl_y);
            break;
        }
        else if (dir_way == 'U' || dir_way == 'D') {
            Verification_Anti_Tank_parcour_vertical(matrice, matrice_mobile, depl_x, depl_y, pos_x, pos_y, succes);
        }
        else if (dir_way == 'L' || dir_way == 'R') {
            Verification_Anti_Tank_parcour_horizontal(matrice, matrice_mobile, depl_x, depl_y, pos_x, pos_y, succes);
        }
        std::cout << "Sur"<< (*matrice)[*depl_y][*depl_x] <<"deplacement x : " << *depl_x << " | deplacement y : " << *depl_y << std::endl;
    } while ((*matrice)[*depl_y][*depl_x] >= Way_U && (*matrice)[*depl_y][*depl_x] <= Way_L);  //Matrice.path --> 15--18

    if (*succes == 0) {
        if ((*matrice)[*depl_y][*depl_x] == Ice) {
            *succes = 2;
            glace(&dir_way, matrice, matrice_mobile, depl_x, depl_y, pos_x, pos_y, succes);
        }else if ((*matrice)[*depl_y][*depl_x] == Thin_Ice){
            *succes = 2;
            glace_fine(&dir_way, matrice, matrice_mobile, depl_x, depl_y, pos_x, pos_y, succes);
        }
        Verification_deplacement(matrice, matrice_mobile, depl_x, depl_y, pos_x, pos_y, succes, direction);
    }
    else if (*succes == -2) {
        *pos_x = *depl_x;
        *pos_y = *depl_y;
    }
}

