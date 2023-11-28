#include "Logique_Jeu.h"

enum Matrice
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

};

//Fonction Engine est appel� dans le main et g�re le fonctionnement de la logique du jeu.
/*Param�tres: S�quence = chaine que l'on va traiter, 
              posX/posY = position du Tank � la foi comme entr�e de position initiale ainsi que position finale
              dir_previuous = entr� et sortie pour savoir dans quel sens est orient� le char
              succes = retourne si on a atteint l'objectif (=1) si mort (=-1) ou toujours en vie (=0)*/
void Engine(int matrice[16][16], const std::string& sequence, int* posX, int* posY, char *dir_previous, int* succes) {
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

//Fonction de verification de la possibilit� d'effectuer le d�placement rentr�
void Verification_deplacement(int matrice[16][16], int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes){

    //V�rification des limites si d�passe pas de mouvement enregistr�
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
    //V�rifie une position valide
    else {
        switch (matrice[*depl_y][*depl_x])
        {
        case Dirt :
        case Bridge:
        case Tank :
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
            Verification_Anti_Tank_parcour_vertical(matrice, depl_x, depl_y, pos_x, pos_y, succes);
            break;
        case Way_D :
            Verification_Anti_Tank_parcour_vertical(matrice, depl_x, depl_y, pos_x, pos_y, succes);
            break;
        case Way_R :
            Verification_Anti_Tank_parcour_horizontal(matrice, depl_x, depl_y, pos_x, pos_y, succes);
            break;
        case Way_L :
            Verification_Anti_Tank_parcour_horizontal(matrice, depl_x, depl_y, pos_x, pos_y, succes);
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
            Portail(matrice, matrice[*depl_y][*depl_x], depl_x, depl_y, succes);
            //Verification anti tank du c�t� du portail de sortie
            Verification_Anti_Tank(matrice, depl_x, depl_y, pos_x, pos_y, succes);
            break;
        default:
            break;
        }
    }

}

//Fonction qui traite le passage au travers un portail afin de sortir le tank au bon endroit
void Portail(int matrice[16][16], int couleur, int* depl_x, int* depl_y, int* succes)
{
    bool sortie = false;

    //Parse la carte pour trouver le premier portail correspondant au portail d'entr�e mais �tant diff�rant.
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            if (matrice[i][j] == couleur && (i != *depl_y || j != *depl_x)) {
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
    if (sortie == false) {
        *succes = -1;
    }
}

//Fonction permettant de v�rifier si un anti tank ne nous tue pas en effectuant le d�placement
void Verification_Anti_Tank(int matrice[16][16], int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes)
{
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
        }

        //Si encore en vie alors deplacement effectu�
        if (*succes != -1) {
            std::cout << "Deplacement OK" << std::endl;
            *pos_x = *depl_x;
            *pos_y = *depl_y;
        }
    
}

//Fonction permettant de v�rifier si un anti tank ne nous tue pas autour d'un parcour continue sur l'axe y (chemin/glace)
void Verification_Anti_Tank_parcour_vertical(int matrice[16][16], int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes)
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

    //Si encore en vie alors deplacement effectu�
    if (*succes != -1) {
        if (matrice[*depl_y][*depl_x]== Way_U) {
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

//Fonction permettant de v�rifier si un anti tank ne nous tue pas autour d'un parcour continue sur l'axe x (chemin/glace)
void Verification_Anti_Tank_parcour_horizontal(int matrice[16][16], int* depl_x, int* depl_y, int* pos_x, int* pos_y, int* succes)
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
    for (int i = 1;  i < *depl_x + 1; i++) {
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

    //Si encore en vie alors deplacement effectu�
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

void Tir()
{
}