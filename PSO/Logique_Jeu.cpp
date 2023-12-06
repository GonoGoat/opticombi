#include "Logique_jeu.h"

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

/*enum Matrice
{
    Dirt, //0
    Tank, //1
    Base, //2
    Water, //3
    Sollid_Block, //4
    Movable_Block, //5
    Bricks, //6
    Anti_Tank_U, Anti_Tank_D, Anti_Tank_R, Anti_Tank_L,
    Mirror_UR, Mirror_UL, Mirror_DR, Mirro_DL,
    Way_U, Way_D, Way_R, Way_L,
    Crystal_Block,
    Rotative_Mirror_UR, Rotative_Mirror_UL, Rotative_Mirror_DR, Rotative_Mirror_DL,
    Ice,
    Thin_Ice,
    Tunnel_Red, Tunnel_Green, Tunnel_Blue, Tunnel_Cyan, Tunnel_Yellow, Tunnel_Pink, Tunnel_White, Tunnel_Dark,
    Bridge

};*/

//Fonction Engine est appelé dans le main et gère le fonctionnement de la logique du jeu.
/*Paramètres: Séquence = chaine que l'on va traiter,
              posX/posY = position du Tank à la foi comme entrée de position initiale ainsi que position finale
              dir_previuous = entré et sortie pour savoir dans quel sens est orienté le char
              succes = retourne si on a atteint l'objectif (=1) si mort (=-1) ou toujours en vie (=0)*/
void Engine(std::vector<std::vector<int>>* matrice, const std::string& sequence, int* posX, int* posY, char* dir_previous, int* succes) {
    //Variables pour traiter le déplacement sans le ressortir directement
    int deplacement_x, deplacement_y;

    //Traite la séquence d'entrée
    for (char dir_actuelle : sequence)
    {
        //Logique de fonctionnement du jeu : 2 direction identiques pour effectuer le déplacement
        if (dir_actuelle == *dir_previous) {

            deplacement_x = *posX;
            deplacement_y = *posY;

            Deplacement(&dir_actuelle, &deplacement_x, &deplacement_y);
            Verification_deplacement(*matrice, &deplacement_x, &deplacement_y, posX, posY, succes);
            //std::cout << "deplacement x : " << *posX << " |deplacement y : " << *posY << std::endl;
            *succes = 0;
        }
        //Tir gérer séparemment car une seul instance suffit pour tirer
        /*else if (dir_actuelle == 'F') {
            std::cout << "Feu en direction " << *dir_previous << std::endl;
            Tir(matrice, *posX, *posY, *dir_previous, succes);
            *succes = 0;
        }*/
        //Enregistre les changement de direction du tank sans le faire bouger sur la carte
        else {
            *dir_previous = dir_actuelle;
        }
    }
}

//Fonction permettant de calculer le déplacement à effectuer sur la particule sur base du caractère de la séquence -> conversion
void Deplacement(char* dir, int* pos_x, int* pos_y) {
    switch (*dir) {
    case 'U':
        *pos_y -= 1;
        break;
    case 'D':
        *pos_y += 1;
        break;
    case 'R':
        //std::cout << "Direction Droite" << std::endl;
        *pos_x += 1;
        break;
    case 'L':
        *pos_x -= 1;
        break;
    }
}

//Fonction de verification de la possibilité d'effectuer le déplacement rentré
void Verification_deplacement(std::vector<std::vector<int>> matrice, int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes) {

    //Vérification des limites si dépasse pas de mouvement enregistré
    if (*depl_x > 15) {
        //std::cout << "Depassement limite droite" << std::endl;
        *depl_x = 15;
    }
    else if (*depl_x < 0) {
        //std::cout << "Depassement limite gauche" << std::endl;
        *depl_x = 0;
    }
    else if (*depl_y > 15) {
        //std::cout << "Depassement limite haute" << std::endl;
        *depl_y = 15;
    }
    else if (*depl_y < 0) {
        //std::cout << "Depassement limite basse" << std::endl;
        *depl_y = 0;
    }
    //Vérifie une position valide
    else {
        switch (matrice[*depl_y][*depl_x])
        {
        case Dirt:
        case Bridge:
        case Tank:
            Verification_Anti_Tank(matrice, depl_x, depl_y, pos_x, pos_y, succes);
            break;
            /**pos_x = *depl_x;
            *pos_y = *depl_y;*/
        case Base:
            /**pos_x = *depl_x;
            *pos_y = *depl_y;*/
            *succes = 1;
            //std::cout << "Base atteinte" << std::endl;

            Verification_Anti_Tank(matrice, depl_x, depl_y, pos_x, pos_y, succes);
            break;
        case Water:
            *succes = -1;
            std::cout << "Mort dans l'eau a la case (" << *depl_x << "," << *depl_y << ")" << std::endl;
            break;
        case Way_U:
            Verification_Anti_Tank_parcour_vertical(matrice, depl_x, depl_y, pos_x, pos_y, succes);
            break;
        case Way_D:
            Verification_Anti_Tank_parcour_vertical(matrice, depl_x, depl_y, pos_x, pos_y, succes);
            break;
        case Way_R:
            Verification_Anti_Tank_parcour_horizontal(matrice, depl_x, depl_y, pos_x, pos_y, succes);
            break;
        case Way_L:
            Verification_Anti_Tank_parcour_horizontal(matrice, depl_x, depl_y, pos_x, pos_y, succes);
            break;
        case Ice:
            break;
        case Thin_Ice:
            break;
        case Tunnel_Red:
        case Tunnel_Green:
        case Tunnel_Blue:
        case Tunnel_Cyan:
        case Tunnel_Yellow:
        case Tunnel_Pink:
        case Tunnel_White:
        case Tunnel_Dark:
            Portail(matrice, matrice[*depl_y][*depl_x], depl_x, depl_y, succes);
            //Verification anti tank du côté du portail de sortie
            Verification_Anti_Tank(matrice, depl_x, depl_y, pos_x, pos_y, succes);
            break;
        default:
            break;
        }
    }

}

//Fonction qui traite le passage au travers un portail afin de sortir le tank au bon endroit
void Portail(std::vector<std::vector<int>> matrice, int couleur, int* depl_x, int* depl_y, int* succes)
{
    bool sortie = false;

    //Parse la carte pour trouver le premier portail correspondant au portail d'entrée mais étant différant.
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            if (matrice[i][j] == couleur && (i != *depl_y || j != *depl_x)) {
                *depl_y = i;
                *depl_x = j;
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
    if (sortie == false) {
        *succes = -1;
    }
}

//Fonction permettant de vérifier si un anti tank ne nous tue pas en effectuant le déplacement
void Verification_Anti_Tank(std::vector<std::vector<int>> matrice, int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes)
{
    //int position = matrice[*depl_y][*depl_x];
    //Verification vertical
            //Au dessus de la position en cours
    /*for (int i = 1; i < *depl_y + 1; i++) {
        position = matrice[*depl_y - i][*depl_x];
        if ((position >= Sollid_Block && position <= Anti_Tank_U) || (position >= Anti_Tank_R && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << position << std::endl;
            break;
        }
        else if (position == Anti_Tank_D) {
            std::cout << "Anti Tank Down" << std::endl;
            *succes = -1;
        }
    }
    //En dessous de la position en cours
    for (int i = 1; i < 16 - *depl_y; i++) {
        position = matrice[*depl_y + i][*depl_x];
        if ((position >= Sollid_Block && position <= Bricks) || (position >= Anti_Tank_D && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_U) {
            std::cout << "Anti Tank UP" << std::endl;
            *succes = -1;
        }
    }

    //Verification horizontal
        //A gauche de la position en cours
    for (int i = 1; i < *depl_x + 1; i++) {
        position = matrice[*depl_y][*depl_x - i];
        if ((position >= Sollid_Block && position <= Anti_Tank_D) || (position >= Anti_Tank_L && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_R) {
            std::cout << "Anti Tank Right" << std::endl;
            *succes = -1;
        }
    }
    //A droite de la position en cours
    for (int i = 1; i < 16 - *depl_x; i++) {
        position = matrice[*depl_y][*depl_x + i];
        if ((position >= Sollid_Block && position <= Anti_Tank_R) || (position >= Mirror_UR && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_L) {
            std::cout << "Anti Tank Left" << std::endl;
            *succes = -1;
        }
    }*/

    *succes = 0;
    //Si encore en vie alors deplacement effectué
    if (*succes != -1) {
        //std::cout << "Deplacement OK" << std::endl;
        *pos_x = *depl_x;
        *pos_y = *depl_y;
    }

}

//Fonction permettant de vérifier si un anti tank ne nous tue pas autour d'un parcour continue sur l'axe y (chemin/glace)
void Verification_Anti_Tank_parcour_vertical(std::vector<std::vector<int>> matrice, int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes)
{
    std::cout << "Verification vertical" << std::endl;
    int position = matrice[*depl_y][*depl_x];
    //Verification vertical
        //Au dessus de la position en cours
    for (int i = 1; i < *depl_y + 1; i++) {
        position = matrice[*depl_y - i][*depl_x];
        if ((position >= Sollid_Block && position <= Anti_Tank_U) || (position >= Anti_Tank_R && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << position << std::endl;
            break;
        }
        else if (position == Anti_Tank_D) {
            std::cout << "Anti Tank Down chemin vert" << std::endl;
            *succes = -1;
        }
    }
    //En dessous de la position en cours
    for (int i = 1; i < 16 - *depl_y; i++) {
        position = matrice[*depl_y + i][*depl_x];
        if ((position >= Sollid_Block && position <= Bricks) || (position >= Anti_Tank_D && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_U) {
            std::cout << "Anti Tank UP chemin vert" << std::endl;
            *succes = -1;
        }
    }

    //Verification horizontal
        //A gauche de la position en cours
    for (int i = *depl_x - 1; i < *depl_x && i > 0; i++) {
        position = matrice[*depl_y][i];
        if ((position >= Sollid_Block && position <= Anti_Tank_D) || (position >= Anti_Tank_L && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_R) {
            std::cout << "Anti Tank Right vert" << std::endl;
            *succes = -1;
        }
    }
    //A droite de la position en cours
    for (int i = *depl_x + 1; i < *depl_x + 2 && i < 15; i++) {
        position = matrice[*depl_y][i];
        if ((position >= Sollid_Block && position <= Anti_Tank_R) || (position >= Mirror_UR && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_L) {
            std::cout << "Anti Tank Left vert" << std::endl;
            *succes = -1;
        }
    }

    //Si encore en vie alors deplacement effectué
    if (*succes != -1) {
        if (matrice[*depl_y][*depl_x] == Way_U) {
            std::cout << "Deplacement chemin UP" << std::endl;
            *pos_x = *depl_x;
            *pos_y = *depl_y - 1;
        }
        else if (matrice[*depl_y][*depl_x] == Way_D) {
            std::cout << "Deplacement chemin Down" << std::endl;
            *pos_x = *depl_x;
            *pos_y = *depl_y + 1;
        }
    }
}

//Fonction permettant de vérifier si un anti tank ne nous tue pas autour d'un parcour continue sur l'axe x (chemin/glace)
void Verification_Anti_Tank_parcour_horizontal(std::vector<std::vector<int>> matrice, int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes)
{
    std::cout << "Verification horizontal" << std::endl;
    int position = matrice[*depl_y][*depl_x];
    //Verification vertical
        //Au dessus de la position en cours
    for (int i = *depl_y - 1; i < *depl_y && i > 0; i++) {
        position = matrice[i][*depl_x];
        if ((position >= Sollid_Block && position <= Anti_Tank_U) || (position >= Anti_Tank_R && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << position << std::endl;
            break;
        }
        else if (position == Anti_Tank_D) {
            std::cout << "Anti Tank Down horiz" << std::endl;
            *succes = -1;
        }
    }
    //En dessous de la position en cours
    for (int i = *depl_y + 1; i < *depl_y + 2 && i < 15; i++) {
        position = matrice[i][*depl_x];
        if ((position >= Sollid_Block && position <= Bricks) || (position >= Anti_Tank_D && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_U) {
            std::cout << "Anti Tank UP horiz" << std::endl;
            *succes = -1;
        }
    }

    //Verification horizontal
        //A gauche de la position en cours
    for (int i = 1; i < *depl_x + 1; i++) {
        position = matrice[*depl_y][*depl_x - i];
        if ((position >= Sollid_Block && position <= Anti_Tank_D) || (position >= Anti_Tank_L && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_R) {
            std::cout << "Anti Tank Right horiz" << std::endl;
            *succes = -1;
        }
    }
    //A droite de la position en cours
    for (int i = 1; i < 16 - *depl_x; i++) {
        position = matrice[*depl_y][*depl_x + i];
        if ((position >= Sollid_Block && position <= Anti_Tank_R) || (position >= Mirror_UR && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_L) {
            std::cout << "Anti Tank Left horiz" << std::endl;
            *succes = -1;
        }
    }

    //Si encore en vie alors deplacement effectué
    if (*succes != -1) {
        if (matrice[*depl_y][*depl_x] == Way_L) {
            std::cout << "Deplacement chemin Left" << std::endl;
            *pos_x = *depl_x - 1;
            *pos_y = *depl_y;
        }
        else if (matrice[*depl_y][*depl_x] == Way_D) {
            std::cout << "Deplacement chemin Right" << std::endl;
            *pos_x = *depl_x + 1;
            *pos_y = *depl_y;
        }
    }
}

//Fonction de calcul de trajectoire
void Tir(std::vector<std::vector<int>> matrice, int depl_x, int depl_y, char dir_laser, int* succes)
{
    bool disparaitre = false;
    int pos_tank_x = depl_x;
    int pos_tank_y = depl_y;
    do
    {
        //Deplacement laser en fonction direction
        switch (dir_laser)
        {
        case 'U':
            depl_y -= 1;
            break;
        case 'D':
            depl_y += 1;
            break;
        case 'R':
            depl_x += 1;
            break;
        case 'L':
            depl_x -= 1;
            break;
        }

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
            switch (matrice[depl_y][depl_x])
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
                std::cout << "Tir arrete" << std::endl;
                disparaitre = true;
                break;
            case Anti_Tank_U:
                if (dir_laser == 'D') {
                    std::cout << "Anti tank Up mort" << std::endl;
                }
                disparaitre = true;
                break;
            case Anti_Tank_D:
                if (dir_laser == 'U') {
                    std::cout << "Anti tank Down mort" << std::endl;
                }
                disparaitre = true;
                break;
            case Anti_Tank_L:
                if (dir_laser == 'R') {
                    std::cout << "Anti tank Left mort" << std::endl;
                }
                disparaitre = true;
                break;
            case Anti_Tank_R:
                if (dir_laser == 'L') {
                    std::cout << "Anti tank Right mort" << std::endl;
                }
                disparaitre = true;
                break;

            default:
                break;
            }
        }

    } while (disparaitre == false);
}