#include "Logique_Jeu.h"

//Variables pour tir laser Anti_tank
int pos_laser_x, pos_laser_y = -1;
bool premiere_case = true;
char dir_anti_tank = 'X';

/**
 * @brief Liste toutes les positions d'un tank lors de l'exécution d'une séquence
 * 
 * @param mapParams Les paramètres de la carte
 * @param svgParams Les paramètres du SVG de sortie
 * @param outputParams Les paramètres du fichiers ltr
 * @param partParams Les paramères de la particule
 */
void getPositionsOfSequence (mapStruct* mapParams, svgStruct* svgParams, outputStruct* outputParams, particleStruct* partParams) {

    // Etablir les positions X et Y de départ
    svgParams->trajX = {partParams->Origine_x};
    svgParams->trajY = {partParams->Origine_y};

    // Initialisation des variables de lancement du moteur
    int success;
    char dir;
    particleStruct part;
    
    // Jusque dernier caractère de séquence
    for(int i = 0;i<partParams->finish_Output.size();i++) {

        // Extraction de la séquence à jouer
        part = *partParams;
        part.finish_Output = partParams->finish_Output.substr(0,i+1);

        // Remise à zéro
        part.Direction_tank = 'U';
        part.success = 0;

        // Jeu de la séquence
        Engine(mapParams,&part);

        // Sauvegarde des positions de fin
        svgParams->trajX.push_back(part.posX);
        svgParams->trajY.push_back(part.posY);
    }
    svgParams->trajSuccess = success;
}

/**
 * @brief Execute la logique du jeu
 * 
 * @param mapParams Les paramètres de la carte
 * @param partParams Les paramères de la particule
 */
void Engine(mapStruct* mapParams,  particleStruct* partParams) {

    //Variables pour traiter le déplacement sans le ressortir directement
    int deplacement_x, deplacement_y;
    //int taille_sequence = 0;

    //Traite la séquence d'entrée
    moveStruct move;
    for (char dir_actuelle : partParams->finish_Output)
    {
        //Logique de fonctionnement du jeu : 2 direction identiques pour effectuer le déplacement
        if (dir_actuelle == partParams->Direction_tank) {

            move.depl_x = partParams->posX;
            move.depl_y = partParams->posY;
            move.dir = dir_actuelle;

            // Déplacement qui tente d'être opéré
            Deplacement(&(move.dir),&(move.depl_x),&(move.depl_y));
            Verification_deplacement(mapParams, &move, partParams);
            std::cout << "deplacement x : " << partParams->posX << " |deplacement y : " << partParams->posY << std::endl;
            partParams->success = 0;
        }
        //Tir gérer séparemment car une seul instance suffit pour tirer
        else if (dir_actuelle == 'F') {
            move.depl_x = partParams->posX;
            move.depl_y = partParams->posY;
            move.dir = partParams->Direction_tank;
            Tir(mapParams, &move, partParams);
            partParams->success = 0;
        }
        //Enregistre les changement de direction du tank sans le faire bouger sur la carte
        else {
            partParams->Direction_tank = dir_actuelle;
        }
        //taille_sequence++;
    }
}

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

/**
 * @brief Fonction de verification de la possibilité d'effectuer le déplacement rentré
 * 
 * @param mapParams Les paramètres de la carte
 * @param moveParams Les paramètres du déplacement désiré
 * @param partParams Les paramères de la particule 
 */
void Verification_deplacement(mapStruct* mapParams, moveStruct* moveParams, particleStruct* partParams) {

    //Vérification des limites si dépasse pas de mouvement enregistré
    if (moveParams->depl_x > mapParams->nbr_colonnes-1) {
        std::cout << "Depassement limite" << std::endl;
        moveParams->depl_x = mapParams->nbr_colonnes-1;
    }
    else if (moveParams->depl_x < 0) {
        moveParams->depl_x = 0;
    }
    else if (moveParams->depl_y > mapParams->nbr_lignes-1) {
        std::cout << "Depassement limite" << std::endl;
        moveParams->depl_y = mapParams->nbr_lignes;
    }
    else if (moveParams->depl_y < 0) {
        moveParams->depl_y = 0;
    }
    //Vérifie une position valide
    else {
        switch (mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] + partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x])
        {
        case Dirt:
        case Bridge:
        case Tank :
            Verification_Anti_Tank(mapParams, moveParams, partParams);
            break;
            /**pos_x = moveParams->depl_x;
            *pos_y = moveParams->depl_y;*/
        case Base:
            /**pos_x = moveParams->depl_x;
            *pos_y = moveParams->depl_y;*/
            partParams->success = 1;
            std::cout << "Base atteinte" << std::endl;

            Verification_Anti_Tank(mapParams, moveParams, partParams);
            break;
        case Water:
            partParams->success = -1;
            std::cout << "Mort dans l'eau a la case (" << moveParams->depl_x << "," << moveParams->depl_y << ")" << std::endl;
            break;
        case Way_U:
        case Way_D:
        case Way_R:
        case Way_L:
            path(mapParams, moveParams, partParams);
            break;
        case Ice:
            glace(mapParams,moveParams, partParams);
            break;
        case Thin_Ice:
            glace_fine(mapParams, moveParams, partParams);
            break;
        case Tunnel_Red:
        case Tunnel_Green:
        case Tunnel_Blue:
        case Tunnel_Cyan:
        case Tunnel_Yellow:
        case Tunnel_Pink:
        case Tunnel_White:
        case Tunnel_Dark:
            Portail (mapParams,moveParams, partParams);
            //Verification anti tank du côté du portail de sortie
            Verification_Anti_Tank(mapParams, moveParams, partParams);
            break;
        default:
            break;
        }
    }

}

/**
 * @brief Gère le déplacement du tank par un portail (tunnel)
 * 
 * @param mapParams Les paramètres de la carte
 * @param moveParams Les paramètres du déplacement désiré
 * @param partParams Les paramères de la particule 
 */
void Portail(mapStruct* mapParams, moveStruct* moveParams, particleStruct* partParams)
{   
    int couleur = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x];
    bool sortie = false;

    //Parse la carte pour trouver le premier portail correspondant au portail d'entrée mais étant différant.
    for (int i = 0; i < mapParams->nbr_lignes; i++) {
        for (int j = 0; j < mapParams->nbr_colonnes; j++) {
            if (mapParams->matrice_fixe[i][j] == couleur && (i != moveParams->depl_y || j != moveParams->depl_x) && (partParams->matrice_mobile[i][j] == Dirt)) {
                moveParams->depl_y = i;
                moveParams->depl_x = j;
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
        partParams->success = -1;
        std::cout << "Mort car pas de sortie au portail" << std::endl;
    }
}

/**
 * @brief Vérifier si un anti tank ne nous tue pas en effectuant le déplacement
 * 
 * @param mapParams Les paramètres de la carte
 * @param moveParams Les paramètres du déplacement désiré
 * @param partParams Les paramères de la particule 
 */
void Verification_Anti_Tank(mapStruct* mapParams, moveStruct* moveParams, particleStruct* partParams)
{
    //!\ ICI utilisation d'une méthode afin de ne pas devoir repenser les conditions car la somme des deux donnera le bon élément (1 des 2 toujours = 0)
    int position = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] + partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x];

    //Verification horizontal
        //A droite de la position en cours
    for (int i = 1; i < mapParams->nbr_colonnes - moveParams->depl_x && partParams->success != -1; i++) {
        position = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x + i] + partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x + i];
        if ((position >= Sollid_Block && position <= Anti_Tank_R) || (position >= Mirror_UR && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_L) {
            partParams->success = -1;
            break;
        }
    }

        //A gauche de la position en cours
    for (int i = 1; i < moveParams->depl_x + 1 && partParams->success != -1; i++) {
        position = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x - i] + partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x - i];
        if ((position >= Sollid_Block && position <= Anti_Tank_D) || (position >= Anti_Tank_L && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_R) {
            partParams->success = -1;
            break;
        }
    }

    //Verification vertical
        //En dessous de la position en cours
    for (int i = 1; i < mapParams->nbr_colonnes - moveParams->depl_y && partParams->success != -1; i++) {
        position = mapParams->matrice_fixe[moveParams->depl_y + i][moveParams->depl_x] + partParams->matrice_mobile[moveParams->depl_y + i][moveParams->depl_x];
        if ((position >= Sollid_Block && position <= Bricks) || (position >= Anti_Tank_D && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_U) {
            partParams->success = -1;
            break;
        }
    }

        //Au dessus de la position en cours
    for (int i = 1; i < moveParams->depl_y + 1 && partParams->success != -1; i++) {
        position = mapParams->matrice_fixe[moveParams->depl_y - i][moveParams->depl_x] + partParams->matrice_mobile[moveParams->depl_y - i][moveParams->depl_x];
        if ((position >= Sollid_Block && position <= Anti_Tank_U) || (position >= Anti_Tank_R && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << position << std::endl;
            break;
        }
        else if (position == Anti_Tank_D) {
            partParams->success = -1;
            break;
        }
    }

    //Si encore en vie alors deplacement effectué
    if (partParams->success != -1) {
        std::cout << "Deplacement OK" << std::endl;
        partParams->posX = moveParams->depl_x;
        partParams->posY = moveParams->depl_y;
    }
}

/**
 * @brief Vérifier si un anti tank ne nous tue pas autour d'un parcour continue sur l'axe y (chemin/glace)
 * 
 * @param mapParams Les paramètres de la carte
 * @param moveParams Les paramètres du déplacement désiré
 * @param partParams Les paramères de la particule 
 */
void Verification_Anti_Tank_parcour_vertical(mapStruct* mapParams, moveStruct* moveParams, particleStruct* partParams)
{
    std::cout << "Verification vertical" << std::endl;
    int position = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] + partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x];
    int distance = 1;

    if (premiere_case) {
        premiere_case = false;
    }
    else {
        distance = 2;
    }

    //Verification horizontal
        //A droite de la position en cours
    for (int i = 1; i < mapParams->nbr_colonnes - moveParams->depl_x && partParams->success != -1 && dir_anti_tank == 'X'; i++) {
        position = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x + i] + partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x + i];
        if ((position >= Sollid_Block && position <= Anti_Tank_R) || (position >= Mirror_UR && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_L) {
            std::cout << "Anti Tank Left vert" << std::endl;
            if (i <= distance) {
                std::cout << "Tue tank" << std::endl;
                partParams->success = -1;
            }
            else {
                dir_anti_tank = 'L';
                pos_laser_x = moveParams->depl_x + i - distance;
                pos_laser_y = moveParams->depl_y;
                std::cout << "Laser x : " << pos_laser_x << " | y : " << pos_laser_y << std::endl;
            }
        }
    }
        //A gauche de la position en cours
    for (int i = 1; i < moveParams->depl_x + 1 && partParams->success != -1 && dir_anti_tank == 'X'; i++) {
        position = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x - i] + partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x - i];
        if ((position >= Sollid_Block && position <= Anti_Tank_D) || (position >= Anti_Tank_L && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_R) {
            std::cout << "Anti Tank Right vert" << std::endl;
            if (i <= distance) {
                std::cout << "Tue tank" << std::endl;
                partParams->success = -1;
            }
            else {
                dir_anti_tank = 'R';
                pos_laser_x = moveParams->depl_x - i + distance;
                pos_laser_y = moveParams->depl_y;
            }
        }
    }

    //Verification vertical
        //En dessous de la position en cours
    for (int i = 1; i < mapParams->nbr_lignes - moveParams->depl_y && partParams->success != -1 && dir_anti_tank == 'X'; i++) {
        position = mapParams->matrice_fixe[moveParams->depl_y + i][moveParams->depl_x] + partParams->matrice_mobile[moveParams->depl_y + i][moveParams->depl_x];
        if ((position >= Sollid_Block && position <= Bricks) || (position >= Anti_Tank_D && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_U) {
            std::cout << "Anti Tank UP chemin vert" << std::endl;
            dir_anti_tank = 'U';
            pos_laser_x = moveParams->depl_x;
            pos_laser_y = moveParams->depl_y + i - distance;
        }
    }
        //Au dessus de la position en cours
    for (int i = 1; i < moveParams->depl_y + 1 && partParams->success != -1 && dir_anti_tank == 'X'; i++) {
        position = mapParams->matrice_fixe[moveParams->depl_y - i][moveParams->depl_x] + partParams->matrice_mobile[moveParams->depl_y - i][moveParams->depl_x];
        if ((position >= Sollid_Block && position <= Anti_Tank_U) || (position >= Anti_Tank_R && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << position << std::endl;
            break;
        }
        else if (position == Anti_Tank_D) {
            std::cout << "Anti Tank Down chemin vert" << std::endl;
            dir_anti_tank = 'D';
            pos_laser_x = moveParams->depl_x;
            pos_laser_y = moveParams->depl_y - i + distance;
        }
    }
}

/**
 * @brief Vérifier si un anti tank ne nous tue pas autour d'un parcour continue sur l'axe x (chemin/glace)
 * 
 * @param mapParams Les paramètres de la carte
 * @param moveParams Les paramètres du déplacement désiré
 * @param partParams Les paramères de la particule 
 */
void Verification_Anti_Tank_parcour_horizontal(mapStruct* mapParams, moveStruct* moveParams, particleStruct* partParams)
{
    std::cout << "Verification horizontal" << std::endl;
    int position = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] + partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x];
    int distance = 1;

    if (premiere_case) {
        premiere_case = false;
    }
    else
    {
        distance = 2;
    }

    //Verification horizontal
            //A droite de la position en cours
    for (int i = 1; i < mapParams->nbr_colonnes - moveParams->depl_x && partParams->success != -1 && dir_anti_tank == 'X'; i++) {
        position = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x + i] + partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x + i];
        if ((position >= Sollid_Block && position <= Anti_Tank_R) || (position >= Mirror_UR && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_L) {
            std::cout << "Anti Tank Left horiz" << std::endl;
            dir_anti_tank = 'L';
            pos_laser_x = moveParams->depl_x + i - distance;
            pos_laser_y = moveParams->depl_y;
            //partParams->success = -1;
        }
    }
        //A gauche de la position en cours
    for (int i = 1; i < moveParams->depl_x + 1 && partParams->success != -1 && dir_anti_tank == 'X'; i++) {
        position = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x - i] + partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x - i];
        if ((position >= Sollid_Block && position <= Anti_Tank_D) || (position >= Anti_Tank_L && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_R) {
            std::cout << "Anti Tank Right horiz" << std::endl;
            dir_anti_tank = 'R';
            pos_laser_x = moveParams->depl_x - i + distance;
            pos_laser_y = moveParams->depl_y;
            //partParams->success = -1;
        }
    }

    //Verification vertical
        //En dessous de la position en cours
    for (int i = 1; i < mapParams->nbr_lignes - moveParams->depl_y && partParams->success != -1 && dir_anti_tank == 'X'; i++) {
        position = mapParams->matrice_fixe[moveParams->depl_y + i][moveParams->depl_x] + partParams->matrice_mobile[moveParams->depl_y + i][moveParams->depl_x];
        if ((position >= Sollid_Block && position <= Bricks) || (position >= Anti_Tank_D && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << Matrice(position) << std::endl;
            break;
        }
        else if (position == Anti_Tank_U) {
            std::cout << "Anti Tank UP horiz" << std::endl;
            if (i <= distance) {
                std::cout << "Tue tank" << std::endl;
                partParams->success = -1;
            }
            else {
                dir_anti_tank = 'U';
                pos_laser_x = moveParams->depl_x;
                pos_laser_y = moveParams->depl_y + i - distance;
            }
        }
    }
        //Au dessus de la position en cours
    for (int i = 1; i < moveParams->depl_y + 1 && partParams->success != -1 && dir_anti_tank == 'X'; i++) {

        position = mapParams->matrice_fixe[moveParams->depl_y - i][moveParams->depl_x] + partParams->matrice_mobile[moveParams->depl_y - i][moveParams->depl_x];
        if ((position >= Sollid_Block && position <= Anti_Tank_U) || (position >= Anti_Tank_R && position <= Mirro_DL) || (position >= Crystal_Block && position < Ice)) {
            //std::cout << "Quitte car " << position << std::endl;
            break;
        }
        else if (position == Anti_Tank_D) {
            std::cout << "Anti Tank Down horiz" << std::endl;
            if (i <= distance) {
                std::cout << "Tue tank" << std::endl;
                partParams->success = -1;
            }
            else {
                dir_anti_tank = 'D';
                pos_laser_x = moveParams->depl_x;
                pos_laser_y = moveParams->depl_y - i + distance;
            }
        }
    }
}

/**
 * @brief Calcul de trajectoire
 * 
 * @param mapParams Les paramètres de la carte
 * @param moveParams Les paramètres du déplacement du laser
 * @param partParams Les paramères de la particule 
 */
void Tir(mapStruct* mapParams, moveStruct* moveParams, particleStruct* partParams)
{
    bool disparaitre = false;
    int pos_tank_x = moveParams->depl_x;
    int pos_tank_y = moveParams->depl_y;
    do
    {
        //Deplacement laser en fonction direction
        Deplacement(&(moveParams->dir),&(moveParams->depl_x),&(moveParams->depl_y));

        //Vérification des limites si dépasse le laser disparait
        if (moveParams->depl_x > mapParams->nbr_colonnes || moveParams->depl_x < 0 || moveParams->depl_y > mapParams->nbr_lignes || moveParams->depl_y < 0) {
            disparaitre = true;
        }
        else if (moveParams->depl_x == pos_tank_x && moveParams->depl_y == pos_tank_y) {
            partParams->success = -1;
            disparaitre = true;
            std::cout << "Auto Kill" << std::endl;
        }
        //Vérifie une position valide
        else {
            switch (mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] + partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x])
            {
            case Mirror_DR:
            case Rotative_Mirror_DR:
                std::cout << "Miroir DR" << std::endl;
                if (moveParams->dir == 'U') {
                    moveParams->dir = 'R';
                }
                else if (moveParams->dir == 'L') {
                    moveParams->dir = 'D';
                }
                else {
                    std::cout << "Tir arrete" << std::endl;
                    disparaitre = true;
                }
                break;
            case Mirro_DL:
            case Rotative_Mirror_DL:
                std::cout << "Miroir DL" << std::endl;
                if (moveParams->dir == 'U') {
                    moveParams->dir = 'L';
                }
                else if (moveParams->dir == 'R') {
                    moveParams->dir = 'D';
                }
                else {
                    std::cout << "Tir arrete" << std::endl;
                    disparaitre = true;
                }
                break;
            case Mirror_UL:
            case Rotative_Mirror_UL:
                std::cout << "Miroir UL" << std::endl;
                if (moveParams->dir == 'D') {
                    moveParams->dir = 'L';
                }
                else if (moveParams->dir == 'R') {
                    moveParams->dir = 'U';
                }
                else {
                    std::cout << "Tir arrete" << std::endl;
                    disparaitre = true;
                }
                break;
            case Mirror_UR:
            case Rotative_Mirror_UR:
                std::cout << "Miroir UR" << std::endl;
                if (moveParams->dir == 'D') {
                    moveParams->dir = 'R';
                }
                else if (moveParams->dir == 'L') {
                    moveParams->dir = 'U';
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
                if (moveParams->dir == 'D') {
                    std::cout << "Anti tank Up mort" << std::endl;
                }
                else {
                    std::cout << "Tir arrete Anti tank Up" << std::endl;
                }
                disparaitre = true;
                break;
            case Anti_Tank_D:
                if (moveParams->dir == 'U') {
                    std::cout << "Anti tank Down mort" << std::endl;
                }
                else {
                    std::cout << "Tir arrete Anti tank Down" << std::endl;
                }
                disparaitre = true;
                break;
            case Anti_Tank_L:
                if (moveParams->dir == 'R') {
                    std::cout << "Anti tank Left mort" << std::endl;
                }
                else {
                    std::cout << "Tir arrete Anti tank Left" << std::endl;
                }
                disparaitre = true;
                break;
            case Anti_Tank_R:
                if (moveParams->dir == 'L') {
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

/**
 * @brief Interraction du tank avec la glace
 * 
 * @param mapParams Les paramètres de la carte
 * @param moveParams Les paramètres du déplacement désiré
 * @param partParams Les paramères de la particule 
 */
void glace(mapStruct* mapParams, moveStruct* moveParams, particleStruct* partParams) {

    std::cout << "Bonjour je suis la fonction Glace!Direction " << moveParams->dir << std::endl;
    int position;

    do {
        if ((moveParams->dir == 'U' || moveParams->dir == 'D') && dir_anti_tank == 'X') {
            Verification_Anti_Tank_parcour_vertical(mapParams, moveParams, partParams);
        }
        else if ((moveParams->dir == 'L' || moveParams->dir == 'R') && dir_anti_tank == 'X') {
            Verification_Anti_Tank_parcour_horizontal(mapParams, moveParams, partParams);
        }
        else if (dir_anti_tank != 'X') {
            tir_Anti_Tank(mapParams, moveParams, partParams);
        }

        if (partParams->success == -1) {
            break;
        }

        Deplacement(&(moveParams->dir),&(moveParams->depl_x),&(moveParams->depl_y));

        position = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] + partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x];

        //Verifie si tank bloque si c'est le cas, il est juste arrêté et sa position est valide
        if (moveParams->depl_x < 0 || moveParams->depl_x>mapParams->nbr_colonnes-1 || moveParams->depl_y < 0 || moveParams->depl_y>mapParams->nbr_lignes-1 || (position >= Sollid_Block && position <= Mirro_DL) || (position >= Crystal_Block && position <= Rotative_Mirror_DL) ) {
            std::cout << "Position non valide" << std::endl;
            partParams->success = -2;
            inverserDirection(&(moveParams->dir));
            Deplacement(&(moveParams->dir),&(moveParams->depl_x),&(moveParams->depl_y));
            inverserDirection(&(moveParams->dir));
            break;
        }
        else if ((position == Way_U && moveParams->dir == 'D') || (position == Way_D && moveParams->dir == 'U') || (position == Way_L && moveParams->dir == 'R') || (position == Way_R && moveParams->dir == 'L')) {
            inverserDirection(&(moveParams->dir));
            Deplacement(&(moveParams->dir),&(moveParams->depl_x),&(moveParams->depl_y));
            inverserDirection(&(moveParams->dir));
            break;
        }

        std::cout << "Sur Glace deplacement x : " << moveParams->depl_x << " |deplacement y : " << moveParams->depl_y << std::endl;
    } while (mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] == Ice && partParams->success != -1);

    if (partParams->success != -1 && partParams->success != -2) {
        if (mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] == Thin_Ice) {
            //Permet d'eviter que le système ne face la vérification finale car les valeurs peuvent être mauvaise (direction)
            partParams->success = 2;
            glace_fine(mapParams, moveParams, partParams);
        }
        else if (mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] >= Way_U && mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] <= Way_L) {
            partParams->success = 2;
            path(mapParams, moveParams, partParams);
        }
        else if (mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] == Ice) {
            partParams->success = 2;
            glace(mapParams, moveParams, partParams);
        }

        if (partParams->success == 0) {
            premiere_case = true;
            if (dir_anti_tank != 'X') {
                tir_Anti_Tank(mapParams,moveParams, partParams);
            }
            Verification_deplacement(mapParams,moveParams, partParams);
        }
        else {
            partParams->success = 0;
        }
    }
    else if (partParams->success == -2) {
        partParams->posX = moveParams->depl_x;
        partParams->posY = moveParams->depl_y;
    }
}

/**
 * @brief Interraction du tank avec la glace fine
 * 
 * @param mapParams Les paramètres de la carte
 * @param moveParams Les paramètres du déplacement désiré
 * @param partParams Les paramères de la particule 
 */
void glace_fine(mapStruct* mapParams, moveStruct* moveParams, particleStruct* partParams) {
    
    std::cout << "Bonjour je suis la fonction Glace fine!Direction " << moveParams->dir << std::endl;
    int position;

    do {
        mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] = Water;

        if ((moveParams->dir == 'U' || moveParams->dir == 'D') && dir_anti_tank == 'X') {
            Verification_Anti_Tank_parcour_vertical(mapParams,moveParams, partParams);
        }
        else if ((moveParams->dir == 'L' || moveParams->dir == 'R') && dir_anti_tank == 'X') {
            Verification_Anti_Tank_parcour_horizontal(mapParams,moveParams, partParams);
        }
        else if (dir_anti_tank != 'X') {
            tir_Anti_Tank(mapParams,moveParams, partParams);
        }

        if (partParams->success == -1) {
            break;
        }

        Deplacement(&(moveParams->dir),&(moveParams->depl_x),&(moveParams->depl_y));

        position = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] + partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x];

        //Verifie si le tank est bloqué dans son mouvement, si c'est le cas il est mort car la glace fine c'est transformé en eau et la séquence est invalide
        if (moveParams->depl_x < 0 || moveParams->depl_x>mapParams->nbr_colonnes || moveParams->depl_y < 0 || moveParams->depl_y>mapParams->nbr_lignes || (position >= Sollid_Block && position <= Mirro_DL) || (position >= Crystal_Block && position <= Rotative_Mirror_DL) || 
            (position == Way_U && moveParams->dir == 'D') || (position == Way_D && moveParams->dir == 'U') || (position == Way_L && moveParams->dir == 'R') || (position == Way_R && moveParams->dir == 'L')) {
            std::cout << "Position non valide, donc mort dans l'eau" << std::endl;
            partParams->success = -1;
            break;
        }
        
        std::cout << "Sur Glace fine deplacement x : " << moveParams->depl_x << " |deplacement y : " << moveParams->depl_y << std::endl;
    } while (mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] == Thin_Ice);

    if (partParams->success != -1 && partParams->success != -2) {
        if (mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] == Ice) {
            //Permet d'eviter que le système ne face la vérification finale car les valeurs peuvent être mauvaise (direction)
            partParams->success = 2;
            glace(mapParams,moveParams, partParams);
        }
        else if (mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] >= Way_U && mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] <= Way_L) {
            partParams->success = 2;
            path(mapParams,moveParams, partParams);
        }

        if (partParams->success == 0) {
            premiere_case = true;
            if (dir_anti_tank != 'X') {
                tir_Anti_Tank(mapParams,moveParams, partParams);
            }
            Verification_deplacement(mapParams,moveParams, partParams);
        }
        else {
            partParams->success = 0;
        }
    }

}

/**
 * @brief Interraction du tank avec les tapis roulants
 * 
 * @param mapParams Les paramètres de la carte
 * @param moveParams Les paramètres du déplacement désiré
 * @param partParams Les paramères de la particule 
 */
void path(mapStruct* mapParams, moveStruct* moveParams, particleStruct* partParams) {
    
    char dir_way;
    int position;

    std::cout << "Bonjour je suis la fonction Path!Direction " << moveParams->dir << std::endl;
    
    do {
        switch (mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x])
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

        if ((dir_way == 'U' || dir_way == 'D') && dir_anti_tank == 'X') {
            Verification_Anti_Tank_parcour_vertical(mapParams,moveParams, partParams);
        }
        else if ((dir_way == 'L' || dir_way == 'R') && dir_anti_tank == 'X') {
            Verification_Anti_Tank_parcour_horizontal(mapParams,moveParams, partParams);
        }
        else if (dir_anti_tank != 'X') {
            tir_Anti_Tank(mapParams, moveParams, partParams);
        }

        if (partParams->success == -1) {
            break;
        }

        Deplacement(&dir_way, &(moveParams->depl_x),&(moveParams->depl_y));

        position = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] + partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x];

        //Verifie si tank bloque si c'est le cas, il est juste arrêté et sa position est valide
        if (moveParams->depl_x < 0 || moveParams->depl_x>mapParams->nbr_colonnes || moveParams->depl_y < 0 || moveParams->depl_y>mapParams->nbr_lignes || (position >= Sollid_Block && position <= Mirro_DL) || (position >= Crystal_Block && position <= Rotative_Mirror_DL)) {
            std::cout << "Position non valide" << std::endl;
            partParams->success = -2;
            inverserDirection(&dir_way);
            Deplacement(&dir_way,&(moveParams->depl_x),&(moveParams->depl_y));
            break;
        }
        
        std::cout << "Sur "<< mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] <<" deplacement x : " << moveParams->depl_x << " | deplacement y : " << moveParams->depl_y << std::endl;
    } while (mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] >= Way_U && mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] <= Way_L);  //Matrice.path --> 15--18

    if (partParams->success != -1 && partParams->success != -2) {
        char dir_backup = moveParams->dir;
        moveParams->dir = dir_way;
        if (mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] == Ice) {
            partParams->success = 2;
            glace(mapParams, moveParams, partParams);
        }else if (mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] == Thin_Ice){
            partParams->success = 2;
            glace_fine(mapParams, moveParams, partParams);
        }
        moveParams->dir = dir_backup;
        if (partParams->success == 0) {
            premiere_case = true;
            if (dir_anti_tank != 'X') {
                tir_Anti_Tank(mapParams, moveParams, partParams);
            }
            Verification_deplacement(mapParams, moveParams, partParams);
        }
        else {
            partParams->success = 0;
        }
    }
    else if (partParams->success == -2) {
        partParams->posX = moveParams->depl_x;
        partParams->posY = moveParams->depl_y;
    }
}

/**
 * @brief Gestion du déplacement du laser tiré par un anti tank
 * 
 * @param mapParams Les paramètres de la carte
 * @param moveParams Les paramètres du déplacement désiré
 * @param partParams Les paramères de la particule 
 */
void tir_Anti_Tank(mapStruct* mapParams, moveStruct* moveParams, particleStruct* partParams)
{
    std::cout << "Trajectoire laser Anti Tank" << std::endl;
    do
    {
        //Deplacement laser en fonction direction
        Deplacement(&dir_anti_tank, &pos_laser_x, &pos_laser_y);

        std::cout << "Laser x : " << pos_laser_x << " | y : " << pos_laser_y << std::endl;

        if (pos_laser_x == moveParams->depl_x && pos_laser_y == moveParams->depl_y) {
            std::cout << "Tank tue" << std::endl;
            partParams->success = -1;
            dir_anti_tank = 'X';
            break;
        }
        //Vérification des limites si dépasse le laser disparait
        else if (pos_laser_x == mapParams->nbr_colonnes || pos_laser_x == 0 || pos_laser_y == mapParams->nbr_lignes || pos_laser_y == 0) {
            dir_anti_tank = 'X';
        }
        //Vérifie une position valide
        else {
            Deplacement(&dir_anti_tank, &pos_laser_x, &pos_laser_y);
            switch (mapParams->matrice_fixe[pos_laser_y][pos_laser_x] + partParams->matrice_mobile[pos_laser_y][pos_laser_x])
            {
            case Mirror_DR:
            case Rotative_Mirror_DR:
                std::cout << "Miroir DR" << std::endl;
                if (dir_anti_tank == 'U') {
                    dir_anti_tank = 'R';
                }
                else if (dir_anti_tank == 'L') {
                    dir_anti_tank = 'D';
                }
                else {
                    std::cout << "Tir arrete" << std::endl;
                    dir_anti_tank = 'X';
                }
                break;
            case Mirro_DL:
            case Rotative_Mirror_DL:
                std::cout << "Miroir DL" << std::endl;
                if (dir_anti_tank == 'U') {
                    dir_anti_tank = 'L';
                }
                else if (dir_anti_tank == 'R') {
                    dir_anti_tank = 'D';
                }
                else {
                    std::cout << "Tir arrete" << std::endl;
                    dir_anti_tank = 'X';
                }
                break;
            case Mirror_UL:
            case Rotative_Mirror_UL:
                std::cout << "Miroir UL" << std::endl;
                if (dir_anti_tank == 'D') {
                    dir_anti_tank = 'L';
                }
                else if (dir_anti_tank == 'R') {
                    dir_anti_tank = 'U';
                }
                else {
                    std::cout << "Tir arrete" << std::endl;
                    dir_anti_tank = 'X';
                }
                break;
            case Mirror_UR:
            case Rotative_Mirror_UR:
                std::cout << "Miroir UR" << std::endl;
                if (dir_anti_tank == 'D') {
                    dir_anti_tank = 'R';
                }
                else if (dir_anti_tank == 'L') {
                    dir_anti_tank = 'U';
                }
                else {
                    std::cout << "Tir arrete" << std::endl;
                    dir_anti_tank = 'X';
                }
                break;
            case Bricks:
            case Sollid_Block:
                std::cout << "Tir arrete Sollid_Block ou Bricks" << std::endl;
                dir_anti_tank = 'X';
                break;
            case Anti_Tank_U:
                if (dir_anti_tank == 'D') {
                    std::cout << "Anti tank Up mort" << std::endl;
                }
                else {
                    std::cout << "Tir arrete Anti tank Up" << std::endl;
                }
                dir_anti_tank = 'X';
                break;
            case Anti_Tank_D:
                if (dir_anti_tank == 'U') {
                    std::cout << "Anti tank Down mort" << std::endl;
                }
                else {
                    std::cout << "Tir arrete Anti tank Down" << std::endl;
                }
                dir_anti_tank = 'X';
                break;
            case Anti_Tank_L:
                if (dir_anti_tank == 'R') {
                    std::cout << "Anti tank Left mort" << std::endl;
                }
                else {
                    std::cout << "Tir arrete Anti tank Left" << std::endl;
                }
                dir_anti_tank = 'X';
                break;
            case Anti_Tank_R:
                if (dir_anti_tank == 'L') {
                    std::cout << "Anti tank Right mort" << std::endl;
                }
                else {
                    std::cout << "Tir arrete Anti tank Right" << std::endl;
                }
                dir_anti_tank = 'X';
                break;

            default:
                break;
            }
            inverserDirection(&dir_anti_tank);
            Deplacement(&dir_anti_tank, &pos_laser_x, &pos_laser_y);
            inverserDirection(&dir_anti_tank);
        }

        //Vérification des limites si dépasse le laser disparait
        /*if (pos_laser_x == 15 || pos_laser_x == 0 || pos_laser_y == 15 || pos_laser_y == 0) {
            dir_anti_tank = 'X';
        }*/

        if (dir_anti_tank == 'X') {
            premiere_case = true;
        }

    } while (dir_anti_tank != 'X' && premiere_case);
}

/**
 * @brief Inverser la direction d'un déplacement
 * 
 * @param direction La direction initiale
 */
void inverserDirection(char* direction) {
    switch (*direction) {
    case 'U':
        *direction = 'D';
        break;
    case 'D':
        *direction = 'U';
        break;
    case 'R':
        *direction = 'L';
        break;
    case 'L':
        *direction = 'R';
        break;
    }
}