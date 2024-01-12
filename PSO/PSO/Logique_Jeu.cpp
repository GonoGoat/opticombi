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
void getPositionsOfSequence(mapStruct* mapParams, svgStruct* svgParams, outputStruct* outputParams, particleStruct* partParams) {

    // Etablir les positions X et Y de départ
    svgParams->trajX = { partParams->Origine_x };
    svgParams->trajY = { partParams->Origine_y };

    // Initialisation des variables de lancement du moteur
    int success = 0;
    char dir;
    particleStruct part;

    // Jusque dernier caractère de séquence
    for (int i = 0; i < partParams->Output.size(); i++) {

        // Extraction de la séquence à jouer
        part = *partParams;
        part.Output = partParams->Output.substr(0, i + 1);

        // Remise à zéro
        part.Direction_tank = 'U';
        part.success = En_vie;

        // Jeu de la séquence
        Engine(mapParams, &part);

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
void Engine(mapStruct* mapParams, particleStruct* partParams) {

    //Variables pour traiter le déplacement sans le ressortir directement
    int deplacement_x, deplacement_y;
    partParams->taille_sequence = 0;

    //Traite la séquence d'entrée
    moveStruct move;
    for (char dir_actuelle : partParams->Output)
    {
        //Logique de fonctionnement du jeu : 2 direction identiques pour effectuer le déplacement
        if (dir_actuelle == partParams->Direction_tank) {
            move.depl_x = partParams->posX;
            move.depl_y = partParams->posY;
            move.dir = dir_actuelle;

            // Déplacement qui tente d'être opéré
            Deplacement(&(move.dir), &(move.depl_x), &(move.depl_y));
            Verification_deplacement(mapParams, &move, partParams);
            //std::cout << "deplacement x : " << partParams->posX << " |deplacement y : " << partParams->posY << std::endl;
        }
        //Tir gérer séparemment car une seul instance suffit pour tirer
        else if (dir_actuelle == 'F') {
            move.depl_x = partParams->posX;
            move.depl_y = partParams->posY;
            move.dir = partParams->Direction_tank;
            Tir(mapParams, &move, partParams);
        }
        //Enregistre les changement de direction du tank sans le faire bouger sur la carte
        else {
            partParams->Direction_tank = dir_actuelle;
        }
        partParams->taille_sequence++;

        if (partParams->success == Mort) {
            //std::cout << "Mort apres "<< partParams->taille_sequence << std::endl;
            partParams->posX = move.depl_x;
            partParams->posY = move.depl_y;
            break;
        }
        else if (partParams->success == Base_atteinte) {
            //std::cout << "Base atteinte" << std::endl;
            break;
        }
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
    if (moveParams->depl_x > mapParams->nbr_colonnes - 1) {
        moveParams->depl_x = mapParams->nbr_colonnes - 1;
    }
    else if (moveParams->depl_x < 0) {
        moveParams->depl_x = 0;
    }
    else if (moveParams->depl_y > mapParams->nbr_lignes - 1) {
        moveParams->depl_y = mapParams->nbr_lignes - 1;
    }
    else if (moveParams->depl_y < 0) {
        moveParams->depl_y = 0;
    }
    //Vérifie une position valide
    else {
        switch (partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x])
        {
        case Dirt:
            switch (mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x])
            {
            case Dirt:
            case Bridge:
            case Tank:
                Verification_Anti_Tank(mapParams, moveParams, partParams);
                break;
            case Base:
                partParams->success = Base_atteinte;
                Verification_Anti_Tank(mapParams, moveParams, partParams);
                break;
            case Water:
                partParams->success = Mort;
                break;
            case Way_U:
            case Way_D:
            case Way_R:
            case Way_L:
                premiere_case = true;
                path(mapParams, moveParams, partParams);
                break;
            case Ice:
                premiere_case = true;
                glace(mapParams, moveParams, partParams);
                break;
            case Tunnel_Red:
            case Tunnel_Green:
            case Tunnel_Blue:
            case Tunnel_Cyan:
            case Tunnel_Yellow:
            case Tunnel_Pink:
            case Tunnel_White:
            case Tunnel_Dark:
                Portail(mapParams, moveParams, partParams);
                //Verification anti tank du côté du portail de sortie
                Verification_Anti_Tank(mapParams, moveParams, partParams);
                break;
            default:
                break;
            }
            break;
        case Bridge:
            Verification_Anti_Tank(mapParams, moveParams, partParams);
            break;
        case Water:
            partParams->success = Mort;
            break;
        case Thin_Ice:
            premiere_case = true;
            glace_fine(mapParams, moveParams, partParams);
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
            if (mapParams->matrice_fixe[i][j] == couleur && (i != moveParams->depl_y || j != moveParams->depl_x)) {
                if ((partParams->matrice_mobile[i][j] == Dirt)) {
                    moveParams->depl_y = i;
                    moveParams->depl_x = j;
                    partParams->success = En_vie;
                    //sortie du parseur quand trouvé le premier portail correspondant
                    sortie = true;
                    break;
                }
                else {
                    partParams->success = Portail_bloque;
                    sortie = true;
                }
            }
        }
        //sortie du parseur quand trouvé le premier portail correspondant
        if (sortie && partParams->success == En_vie) {
            break;
        }
    }
    //Si pas sorti par un tunnel alors mort.
    if (sortie == false) {
        partParams->success = Mort;
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
    int position_fixe = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x];
    int position_mobile = partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x];

    //Verification horizontal
        //A droite de la position en cours
    for (int i = 1; i < mapParams->nbr_colonnes - moveParams->depl_x && partParams->success != Mort; i++) {
        position_fixe = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x + i];
        position_mobile = partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x + i];
        if (position_fixe == Sollid_Block || position_fixe == Crystal_Block || (position_mobile >= Movable_Block && position_mobile <= Anti_Tank_R) || (position_mobile >= Anti_Tank_Dead_U && position_mobile <= Rotative_Mirror_DL)) {
            break;
        }
        else if (position_mobile == Anti_Tank_L) {
            partParams->success = Mort;
            break;
        }
    }

    //A gauche de la position en cours
    for (int i = 1; i < moveParams->depl_x + 1 && partParams->success != Mort; i++) {
        position_fixe = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x - i];
        position_mobile = partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x - i];
        if (position_fixe == Sollid_Block || position_fixe == Crystal_Block || (position_mobile >= Movable_Block && position_mobile <= Anti_Tank_D) || (position_mobile >= Anti_Tank_L && position_mobile <= Rotative_Mirror_DL)) {
            break;
        }
        else if (position_mobile == Anti_Tank_R) {
            partParams->success = Mort;
            break;
        }
    }

    //Verification vertical
        //En dessous de la position en cours
    for (int i = 1; i < mapParams->nbr_colonnes - moveParams->depl_y && partParams->success != Mort; i++) {
        position_fixe = mapParams->matrice_fixe[moveParams->depl_y + i][moveParams->depl_x];
        position_mobile = partParams->matrice_mobile[moveParams->depl_y + i][moveParams->depl_x];
        if (position_fixe == Sollid_Block || position_fixe == Crystal_Block || (position_mobile >= Movable_Block && position_mobile <= Bricks) || (position_mobile >= Anti_Tank_D && position_mobile <= Rotative_Mirror_DL)) {
            break;
        }
        else if (position_mobile == Anti_Tank_U) {
            partParams->success = Mort;
            break;
        }
    }

    //Au dessus de la position en cours
    for (int i = 1; i < moveParams->depl_y + 1 && partParams->success != Mort; i++) {
        position_fixe = mapParams->matrice_fixe[moveParams->depl_y - i][moveParams->depl_x];
        position_mobile = partParams->matrice_mobile[moveParams->depl_y - i][moveParams->depl_x];
        if (position_fixe == Sollid_Block || position_fixe == Crystal_Block || (position_mobile >= Movable_Block && position_mobile <= Anti_Tank_U) || (position_mobile >= Anti_Tank_R && position_mobile <= Rotative_Mirror_DL)) {
            break;
        }
        else if (position_mobile == Anti_Tank_D) {
            partParams->success = Mort;
            break;
        }
    }

    //Si encore en vie alors deplacement effectué
    if (partParams->success != Mort) {
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
    int position_fixe = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x];
    int position_mobile = partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x];
    int distance = 1;

    if (premiere_case) {
        premiere_case = false;
    }
    else {
        distance = 2;
    }

    //Verification horizontal
        //A droite de la position en cours
    for (int i = 1; i < mapParams->nbr_colonnes - moveParams->depl_x && partParams->success != Mort && dir_anti_tank == 'X'; i++) {
        position_fixe = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x + i];
        position_mobile = partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x + i];
        if (position_fixe == Sollid_Block || position_fixe == Crystal_Block || (position_mobile >= Movable_Block && position_mobile <= Anti_Tank_R) || (position_mobile >= Anti_Tank_Dead_U && position_mobile <= Rotative_Mirror_DL)) {
            break;
        }
        else if (position_mobile == Anti_Tank_L) {
            if (i <= distance) {
                //std::cout << "Tue par anti tank L situe en x "<< moveParams->depl_x + i << " | y " << moveParams->depl_y << std::endl;
                partParams->success = Mort;
            }
            else {
                dir_anti_tank = 'L';
                pos_laser_x = moveParams->depl_x + i - distance;
                pos_laser_y = moveParams->depl_y;
            }
        }
    }
    //A gauche de la position en cours
    for (int i = 1; i < moveParams->depl_x + 1 && partParams->success != Mort && dir_anti_tank == 'X'; i++) {
        position_fixe = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x - i];
        position_mobile = partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x - i];
        if (position_fixe == Sollid_Block || position_fixe == Crystal_Block || (position_mobile >= Movable_Block && position_mobile <= Anti_Tank_D) || (position_mobile >= Anti_Tank_L && position_mobile <= Rotative_Mirror_DL)) {
            break;
        }
        else if (position_mobile == Anti_Tank_R) {
            if (i <= distance) {
                //std::cout << "Tue par anti tank R" << std::endl;
                partParams->success = Mort;
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
    for (int i = 1; i < mapParams->nbr_lignes - moveParams->depl_y && partParams->success != Mort && dir_anti_tank == 'X'; i++) {
        position_fixe = mapParams->matrice_fixe[moveParams->depl_y + i][moveParams->depl_x];
        position_mobile = partParams->matrice_mobile[moveParams->depl_y + i][moveParams->depl_x];
        if (position_fixe == Sollid_Block || position_fixe == Crystal_Block || (position_mobile >= Movable_Block && position_mobile <= Bricks) || (position_mobile >= Anti_Tank_D && position_mobile <= Rotative_Mirror_DL)) {
            break;
        }
        else if (position_mobile == Anti_Tank_U) {
            dir_anti_tank = 'U';
            pos_laser_x = moveParams->depl_x;
            pos_laser_y = moveParams->depl_y + i - distance;
        }
    }
    //Au dessus de la position en cours
    for (int i = 1; i < moveParams->depl_y + 1 && partParams->success != Mort && dir_anti_tank == 'X'; i++) {
        position_fixe = mapParams->matrice_fixe[moveParams->depl_y - i][moveParams->depl_x];
        position_mobile = partParams->matrice_mobile[moveParams->depl_y - i][moveParams->depl_x];
        if (position_fixe == Sollid_Block || position_fixe == Crystal_Block || (position_mobile >= Movable_Block && position_mobile <= Anti_Tank_U) || (position_mobile >= Anti_Tank_R && position_mobile <= Rotative_Mirror_DL)) {
            break;
        }
        else if (position_mobile == Anti_Tank_D) {
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
    int position_fixe = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x];
    int position_mobile = partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x];
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
    for (int i = 1; i < mapParams->nbr_colonnes - moveParams->depl_x && partParams->success != Mort && dir_anti_tank == 'X'; i++) {
        position_fixe = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x + i];
        position_mobile = partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x + i];
        if (position_fixe == Sollid_Block || position_fixe == Crystal_Block || (position_mobile >= Movable_Block && position_mobile <= Anti_Tank_R) || (position_mobile >= Anti_Tank_Dead_U && position_mobile <= Rotative_Mirror_DL)) {
            break;
        }
        else if (position_mobile == Anti_Tank_L) {
            dir_anti_tank = 'L';
            pos_laser_x = moveParams->depl_x + i - distance;
            pos_laser_y = moveParams->depl_y;
        }
    }
    //A gauche de la position en cours
    for (int i = 1; i < moveParams->depl_x + 1 && partParams->success != Mort && dir_anti_tank == 'X'; i++) {
        position_fixe = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x - i];
        position_mobile = partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x - i];
        if (position_fixe == Sollid_Block || position_fixe == Crystal_Block || (position_mobile >= Movable_Block && position_mobile <= Anti_Tank_D) || (position_mobile >= Anti_Tank_L && position_mobile <= Rotative_Mirror_DL)) {
            break;
        }
        else if (position_mobile == Anti_Tank_R) {
            dir_anti_tank = 'R';
            pos_laser_x = moveParams->depl_x - i + distance;
            pos_laser_y = moveParams->depl_y;
        }
    }

    //Verification vertical
        //En dessous de la position en cours
    for (int i = 1; i < mapParams->nbr_lignes - moveParams->depl_y && partParams->success != Mort && dir_anti_tank == 'X'; i++) {
        position_fixe = mapParams->matrice_fixe[moveParams->depl_y + i][moveParams->depl_x];
        position_mobile = partParams->matrice_mobile[moveParams->depl_y + i][moveParams->depl_x];
        if (position_fixe == Sollid_Block || position_fixe == Crystal_Block || (position_mobile >= Movable_Block && position_mobile <= Bricks) || (position_mobile >= Anti_Tank_D && position_mobile <= Rotative_Mirror_DL)) {
            break;
        }
        else if (position_mobile == Anti_Tank_U) {
            if (i <= distance) {
                //std::cout << "Tue par anti tank U" << std::endl;
                partParams->success = Mort;
            }
            else {
                dir_anti_tank = 'U';
                pos_laser_x = moveParams->depl_x;
                pos_laser_y = moveParams->depl_y + i - distance;
            }
        }
    }
    //Au dessus de la position en cours
    for (int i = 1; i < moveParams->depl_y + 1 && partParams->success != Mort && dir_anti_tank == 'X'; i++) {

        position_fixe = mapParams->matrice_fixe[moveParams->depl_y - i][moveParams->depl_x];
        position_mobile = partParams->matrice_mobile[moveParams->depl_y - i][moveParams->depl_x];
        if (position_fixe == Sollid_Block || position_fixe == Crystal_Block || (position_mobile >= Movable_Block && position_mobile <= Anti_Tank_U) || (position_mobile >= Anti_Tank_R && position_mobile <= Rotative_Mirror_DL)) {
            break;
        }
        else if (position_mobile == Anti_Tank_D) {
            if (i <= distance) {
                //std::cout << "Tue par anti tank D" << std::endl;
                partParams->success = Mort;
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

    mobileStruct bloc_mobile;
    bloc_mobile.dir = moveParams->dir;
    bloc_mobile.depl_x = moveParams->depl_x; 
    bloc_mobile.depl_y = moveParams->depl_y;
    
    do
    {
        //Deplacement laser en fonction direction
        Deplacement(&(bloc_mobile.dir), &(bloc_mobile.depl_x), &(bloc_mobile.depl_y));

        //Vérification des limites si dépasse le laser disparait
        if (bloc_mobile.depl_x > mapParams->nbr_colonnes - 1 || bloc_mobile.depl_x < 0 || bloc_mobile.depl_y > mapParams->nbr_lignes - 1 || bloc_mobile.depl_y < 0) {
            disparaitre = true;
        }
        else if (bloc_mobile.depl_x == moveParams->depl_x && bloc_mobile.depl_y == moveParams->depl_y) {
            partParams->success = Mort;
            disparaitre = true;
        }
        else if (mapParams->matrice_fixe[bloc_mobile.depl_y][bloc_mobile.depl_x] == Sollid_Block) {
            disparaitre = true;
        }
        //Vérifie une position valide
        else {
            switch (partParams->matrice_mobile[bloc_mobile.depl_y][bloc_mobile.depl_x])
            {
            case Mirror_DR:
                if (bloc_mobile.dir == 'U') {
                    bloc_mobile.dir = 'R';
                }
                else if (bloc_mobile.dir == 'L') {
                    bloc_mobile.dir = 'D';
                }
                else {
                    verification_Bloc_Mobile(mapParams, &bloc_mobile, partParams);
                    disparaitre = true;
                }
                break;
            case Rotative_Mirror_DR:
                if (bloc_mobile.dir == 'U') {
                    bloc_mobile.dir = 'R';
                }
                else if (bloc_mobile.dir == 'L') {
                    bloc_mobile.dir = 'D';
                }
                else {
                    partParams->matrice_mobile[bloc_mobile.depl_y][bloc_mobile.depl_x] = Rotative_Mirror_DL;
                    disparaitre = true;
                }
                break;
            case Mirror_DL:
                if (bloc_mobile.dir == 'U') {
                    bloc_mobile.dir = 'L';
                }
                else if (bloc_mobile.dir == 'R') {
                    bloc_mobile.dir = 'D';
                }
                else {
                    verification_Bloc_Mobile(mapParams, &bloc_mobile, partParams);
                    disparaitre = true;
                }
                break;
            case Rotative_Mirror_DL:
                if (bloc_mobile.dir == 'U') {
                    bloc_mobile.dir = 'L';
                }
                else if (bloc_mobile.dir == 'R') {
                    bloc_mobile.dir = 'D';
                }
                else {
                    partParams->matrice_mobile[bloc_mobile.depl_y][bloc_mobile.depl_x] = Rotative_Mirror_UL;
                    disparaitre = true;
                }
                break;
            case Mirror_UL:
                if (bloc_mobile.dir == 'D') {
                    bloc_mobile.dir = 'L';
                }
                else if (bloc_mobile.dir == 'R') {
                    bloc_mobile.dir = 'U';
                }
                else {
                    verification_Bloc_Mobile(mapParams, &bloc_mobile, partParams);
                    disparaitre = true;
                }
                break;
            case Rotative_Mirror_UL:
                if (bloc_mobile.dir == 'D') {
                    bloc_mobile.dir = 'L';
                }
                else if (bloc_mobile.dir == 'R') {
                    bloc_mobile.dir = 'U';
                }
                else {
                    partParams->matrice_mobile[bloc_mobile.depl_y][bloc_mobile.depl_x] = Rotative_Mirror_UR;
                    disparaitre = true;
                }
                break;
            case Mirror_UR:
                if (bloc_mobile.dir == 'D') {
                    bloc_mobile.dir = 'R';
                }
                else if (bloc_mobile.dir == 'L') {
                    bloc_mobile.dir = 'U';
                }
                else {
                    verification_Bloc_Mobile(mapParams, &bloc_mobile, partParams);
                    disparaitre = true;
                }
                break;
            case Rotative_Mirror_UR:
                if (bloc_mobile.dir == 'D') {
                    bloc_mobile.dir = 'R';
                }
                else if (bloc_mobile.dir == 'L') {
                    bloc_mobile.dir = 'U';
                }
                else {
                    partParams->matrice_mobile[bloc_mobile.depl_y][bloc_mobile.depl_x] = Rotative_Mirror_DR;
                    disparaitre = true;
                }
                break;
            case Bricks:
                partParams->matrice_mobile[bloc_mobile.depl_y][bloc_mobile.depl_x] = Dirt;
                disparaitre = true;
                break;
            case Anti_Tank_U:
            case Anti_Tank_Dead_U:
                if (bloc_mobile.dir == 'D') {
                    partParams->matrice_mobile[bloc_mobile.depl_y][bloc_mobile.depl_x] = Anti_Tank_Dead_U;
                }
                else {
                    verification_Bloc_Mobile(mapParams, &bloc_mobile, partParams);
                }
                disparaitre = true;
                break;
            case Anti_Tank_D:
            case Anti_Tank_Dead_D :
                if (bloc_mobile.dir == 'U') {
                    partParams->matrice_mobile[bloc_mobile.depl_y][bloc_mobile.depl_x] = Anti_Tank_Dead_D;
                }
                else {
                    verification_Bloc_Mobile(mapParams, &bloc_mobile, partParams);
                }
                disparaitre = true;
                break;
            case Anti_Tank_L:
            case Anti_Tank_Dead_L:
                if (bloc_mobile.dir == 'R') {
                    partParams->matrice_mobile[bloc_mobile.depl_y][bloc_mobile.depl_x] = Anti_Tank_Dead_L;
                }
                else {
                    verification_Bloc_Mobile(mapParams, &bloc_mobile, partParams);
                }
                disparaitre = true;
                break;
            case Anti_Tank_R:
            case Anti_Tank_Dead_R:
                if (bloc_mobile.dir == 'L') {
                    partParams->matrice_mobile[bloc_mobile.depl_y][bloc_mobile.depl_x] = Anti_Tank_Dead_R;
                }
                else {
                    verification_Bloc_Mobile(mapParams, &bloc_mobile, partParams);
                }
                disparaitre = true;
                break;
            case Movable_Block:
                verification_Bloc_Mobile(mapParams, &bloc_mobile, partParams);
                disparaitre = true;
                break;
            default:
                break;
            }
        }

    } while (disparaitre == false);

    if (partParams->success == Portail_bloque) {
        Portail(mapParams, moveParams, partParams);
    }
    Verification_Anti_Tank(mapParams, moveParams, partParams);
}

/**
 * @brief Interraction du tank avec la glace
 *
 * @param mapParams Les paramètres de la carte
 * @param moveParams Les paramètres du déplacement désiré
 * @param partParams Les paramères de la particule
 */
void glace(mapStruct* mapParams, moveStruct* moveParams, particleStruct* partParams) {

    int position_fixe;
    int position_mobile;

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

        if (partParams->success == Mort) {
            break;
        }

        Deplacement(&(moveParams->dir), &(moveParams->depl_x), &(moveParams->depl_y));

        //Verifie si tank bloque si c'est le cas, il est juste arrêté et sa position est valide
        if ((moveParams->depl_x < 0 && moveParams->dir == 'L') || (moveParams->depl_x > mapParams->nbr_colonnes - 1 && moveParams->dir == 'R') || (moveParams->depl_y < 0 && moveParams->dir == 'U') || (moveParams->depl_y > mapParams->nbr_lignes - 1 && moveParams->dir == 'D')) {
            if (partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x] == Thin_Ice) {
                partParams->success = Mort;
                partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x] = Water;
                break;
            }
            else {
                partParams->success = Position_non_valide;
                break;
            }
        }
        position_fixe = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x];
        position_mobile = partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x];
        if (moveParams->depl_x < 0 || moveParams->depl_x>mapParams->nbr_colonnes - 1 || moveParams->depl_y < 0 || moveParams->depl_y>mapParams->nbr_lignes - 1 || position_fixe == Sollid_Block || position_fixe == Crystal_Block || (position_mobile >= Movable_Block && position_fixe <= Rotative_Mirror_DL)) {
            partParams->success = Position_non_valide;
            inverserDirection(&(moveParams->dir));
            Deplacement(&(moveParams->dir), &(moveParams->depl_x), &(moveParams->depl_y));
            inverserDirection(&(moveParams->dir));
            break;
        }
        else if ((position_fixe == Way_U && moveParams->dir == 'D') || (position_fixe == Way_D && moveParams->dir == 'U') || (position_fixe == Way_L && moveParams->dir == 'R') || (position_fixe == Way_R && moveParams->dir == 'L')) {
            inverserDirection(&(moveParams->dir));
            Deplacement(&(moveParams->dir), &(moveParams->depl_x), &(moveParams->depl_y));
            inverserDirection(&(moveParams->dir));
            break;
        }
    } while (mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] == Ice && partParams->success != Mort);

    if (partParams->success != Mort && partParams->success != Position_non_valide) {
        if (mapParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x] == Thin_Ice) {
            //Permet d'eviter que le système ne face la vérification finale car les valeurs peuvent être mauvaise (direction)
            partParams->success = Passage_chemin;
            glace_fine(mapParams, moveParams, partParams);
        }
        else if (mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] >= Way_U && mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] <= Way_L) {
            partParams->success = Passage_chemin;
            path(mapParams, moveParams, partParams);
        }
        else if (mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] == Ice) {
            partParams->success = Passage_chemin;
            glace(mapParams, moveParams, partParams);
        }

        if (partParams->success == En_vie) {
            premiere_case = true;
            if (dir_anti_tank != 'X') {
                tir_Anti_Tank(mapParams, moveParams, partParams);
            }
            Verification_deplacement(mapParams, moveParams, partParams);
        }
        else {
            partParams->success = En_vie;
        }
    }
    else if (partParams->success == Position_non_valide) {
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

    int position_fixe;
    int position_mobile;

    do {
        partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x] = Water;

        if ((moveParams->dir == 'U' || moveParams->dir == 'D') && dir_anti_tank == 'X') {
            Verification_Anti_Tank_parcour_vertical(mapParams, moveParams, partParams);
        }
        else if ((moveParams->dir == 'L' || moveParams->dir == 'R') && dir_anti_tank == 'X') {
            Verification_Anti_Tank_parcour_horizontal(mapParams, moveParams, partParams);
        }
        else if (dir_anti_tank != 'X') {
            tir_Anti_Tank(mapParams, moveParams, partParams);
        }

        if (partParams->success == Mort) {
            break;
        }

        Deplacement(&(moveParams->dir), &(moveParams->depl_x), &(moveParams->depl_y));

        if ((moveParams->depl_x < 0 && moveParams->dir == 'L') || (moveParams->depl_x > mapParams->nbr_colonnes - 1 && moveParams->dir == 'R') || (moveParams->depl_y < 0 && moveParams->dir == 'U') || (moveParams->depl_y > mapParams->nbr_lignes - 1 && moveParams->dir == 'D')) {
            partParams->success = Mort;
            partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x] = Water;
            break;
        }

        position_fixe = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x];
        position_mobile = partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x];

        //Verifie si le tank est bloqué dans son mouvement, si c'est le cas il est mort car la glace fine c'est transformé en eau et la séquence est invalide
        if (position_fixe == Sollid_Block || position_fixe == Crystal_Block || (position_mobile >= Movable_Block && position_mobile <= Rotative_Mirror_DL) || (position_mobile == Way_U && moveParams->dir == 'D')
            || (position_fixe == Way_D && moveParams->dir == 'U') || (position_fixe == Way_L && moveParams->dir == 'R') || (position_fixe == Way_R && moveParams->dir == 'L')) {
            partParams->success = Mort;
            partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x] = Water;
            break;
        }

    } while (partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x] == Thin_Ice);

    if (partParams->success != Mort && partParams->success != Position_non_valide) {
        if (mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] == Ice) {
            //Permet d'eviter que le système ne face la vérification finale car les valeurs peuvent être mauvaise (direction)
            partParams->success = Passage_chemin;
            glace(mapParams, moveParams, partParams);
        }
        else if (mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] >= Way_U && mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] <= Way_L) {
            partParams->success = Passage_chemin;
            path(mapParams, moveParams, partParams);
        }

        if (partParams->success == En_vie) {
            premiere_case = true;
            if (dir_anti_tank != 'X') {
                tir_Anti_Tank(mapParams, moveParams, partParams);
            }
            Verification_deplacement(mapParams, moveParams, partParams);
        }
        else {
            partParams->success = En_vie;
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
    int position_fixe;
    int position_mobile;

    do {
        switch (mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x])
        {
        case Way_U:
            dir_way = 'U';
            break;
        case Way_D:
            dir_way = 'D';
            break;
        case Way_R:
            dir_way = 'R';
            break;
        case Way_L:
            dir_way = 'L';
            break;
        default:
            break;
        }

        if ((dir_way == 'U' || dir_way == 'D') && dir_anti_tank == 'X') {
            Verification_Anti_Tank_parcour_vertical(mapParams, moveParams, partParams);
        }
        else if ((dir_way == 'L' || dir_way == 'R') && dir_anti_tank == 'X') {
            Verification_Anti_Tank_parcour_horizontal(mapParams, moveParams, partParams);
        }
        else if (dir_anti_tank != 'X') {
            tir_Anti_Tank(mapParams, moveParams, partParams);
        }

        if (partParams->success == Mort) {
            break;
        }

        Deplacement(&dir_way, &(moveParams->depl_x), &(moveParams->depl_y));

        //Verifie si tank bloque si c'est le cas, il est juste arrêté et sa position est valide
        if ((moveParams->depl_x < 0 && dir_way == 'L') || (moveParams->depl_x > mapParams->nbr_colonnes - 1 && dir_way == 'R') || (moveParams->depl_y < 0 && dir_way == 'U') || (moveParams->depl_y > mapParams->nbr_lignes - 1 && dir_way == 'D')) {
            //std::cout << "Arrete par les limites du jeu en x "<< moveParams->depl_x<< " | en y "<< moveParams->depl_y << std::endl;
            if (partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x] == Thin_Ice) {
                partParams->success = Mort;
                partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x] = Water;
                break;
            }
            else {
                partParams->success = Position_non_valide;
                break;
            }
        }

        position_fixe = mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x]; 
        position_mobile = partParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x];
        
        if (position_fixe == Sollid_Block || position_fixe == Crystal_Block || (position_mobile >= Movable_Block && position_mobile <= Rotative_Mirror_DL)) {
            partParams->success = Position_non_valide;
            inverserDirection(&dir_way);
            Deplacement(&dir_way, &(moveParams->depl_x), &(moveParams->depl_y));
            break;
        }
    } while (mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] >= Way_U && mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] <= Way_L);  //Matrice.path --> 15--18

    if (partParams->success != Mort && partParams->success != Position_non_valide) {
        char dir_backup = moveParams->dir;
        moveParams->dir = dir_way;
        if (mapParams->matrice_fixe[moveParams->depl_y][moveParams->depl_x] == Ice) {
            partParams->success = Passage_chemin;
            glace(mapParams, moveParams, partParams);
        }
        else if (mapParams->matrice_mobile[moveParams->depl_y][moveParams->depl_x] == Thin_Ice) {
            partParams->success = Passage_chemin;
            glace_fine(mapParams, moveParams, partParams);
        }
        moveParams->dir = dir_backup;
        if (partParams->success == En_vie) {
            premiere_case = true;
            if (dir_anti_tank != 'X') {
                tir_Anti_Tank(mapParams, moveParams, partParams);
            }
            Verification_deplacement(mapParams, moveParams, partParams);
        }
        else {
            partParams->success = En_vie;
        }
    }
    else if (partParams->success == Position_non_valide) {
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
    mobileStruct bloc_mobile;

    do
    {
        //Deplacement laser en fonction direction
        Deplacement(&dir_anti_tank, &pos_laser_x, &pos_laser_y);

        if (pos_laser_x == moveParams->depl_x && pos_laser_y == moveParams->depl_y) {
            partParams->success = Mort;
            //std::cout << "Laser rencontre Tank - Mort" << std::endl;
            dir_anti_tank = 'X';
            break;
        }
        //Vérification des limites si dépasse le laser disparait
        else if (((pos_laser_x >= mapParams->nbr_colonnes - 1 && dir_anti_tank == 'R') || (pos_laser_x <= 0 && dir_anti_tank == 'L') || (pos_laser_y >= mapParams->nbr_lignes - 1 && dir_anti_tank == 'D') || (pos_laser_y <= 0 && dir_anti_tank == 'U')) && partParams->matrice_mobile[pos_laser_y][pos_laser_x] == Dirt) {
            //std::cout << "Laser atteint limites en x "<< pos_laser_x <<" | y "<< pos_laser_y << std::endl;
            dir_anti_tank = 'X';
        }
        else {
            Deplacement(&dir_anti_tank, &pos_laser_x, &pos_laser_y);
            if (mapParams->matrice_fixe[pos_laser_y][pos_laser_x] == Sollid_Block) {
                //std::cout << "Laser atteint bloc solide" << std::endl;
                dir_anti_tank = 'X';
            }
            //Vérifie une position valide
            else {
                bloc_mobile.depl_x = pos_laser_x;
                bloc_mobile.depl_y = pos_laser_y;
                bloc_mobile.dir = dir_anti_tank;
                switch (partParams->matrice_mobile[pos_laser_y][pos_laser_x])
                {
                case Mirror_DR:
                    if (dir_anti_tank == 'U') {
                        dir_anti_tank = 'R';
                    }
                    else if (dir_anti_tank == 'L') {
                        dir_anti_tank = 'D';
                    }
                    else {
                        //std::cout << "Laser arrete Mirroir DR" << std::endl;
                        verification_Bloc_Mobile(mapParams, &bloc_mobile, partParams);
                        dir_anti_tank = 'X';
                    }
                    break;
                case Rotative_Mirror_DR:
                    if (dir_anti_tank == 'U') {
                        dir_anti_tank = 'R';
                    }
                    else if (dir_anti_tank == 'L') {
                        dir_anti_tank = 'D';
                    }
                    else {
                        //std::cout << "Laser arrete Rotative Mirroir DR" << std::endl;
                        partParams->matrice_mobile[pos_laser_y][pos_laser_x] = Rotative_Mirror_DL;
                        dir_anti_tank = 'X';
                    }
                    break;
                case Mirror_DL:
                    if (dir_anti_tank == 'U') {
                        dir_anti_tank = 'L';
                    }
                    else if (dir_anti_tank == 'R') {
                        dir_anti_tank = 'D';
                    }
                    else {
                        //std::cout << "Laser arrete Mirroir DL" << std::endl;
                        verification_Bloc_Mobile(mapParams, &bloc_mobile, partParams);
                        dir_anti_tank = 'X';
                    }
                    break;
                case Rotative_Mirror_DL:
                    if (dir_anti_tank == 'U') {
                        dir_anti_tank = 'L';
                    }
                    else if (dir_anti_tank == 'R') {
                        dir_anti_tank = 'D';
                    }
                    else {
                        //std::cout << "Laser arrete Rotative Mirroir DL" << std::endl;
                        partParams->matrice_mobile[pos_laser_y][pos_laser_x] = Rotative_Mirror_UL;
                        dir_anti_tank = 'X';
                    }
                    break;
                case Mirror_UL:
                    if (dir_anti_tank == 'D') {
                        dir_anti_tank = 'L';
                    }
                    else if (dir_anti_tank == 'R') {
                        dir_anti_tank = 'U';
                    }
                    else {
                        //std::cout << "Laser arrete Mirroir UL" << std::endl;
                        verification_Bloc_Mobile(mapParams, &bloc_mobile, partParams);
                        dir_anti_tank = 'X';
                    }
                    break;
                case Rotative_Mirror_UL:
                    if (dir_anti_tank == 'D') {
                        dir_anti_tank = 'L';
                    }
                    else if (dir_anti_tank == 'R') {
                        dir_anti_tank = 'U';
                    }
                    else {
                        //std::cout << "Laser arrete Rotative Mirroir UL" << std::endl;
                        partParams->matrice_mobile[pos_laser_y][pos_laser_x] = Rotative_Mirror_UR;
                        dir_anti_tank = 'X';
                    }
                    break;
                case Mirror_UR:
                    if (dir_anti_tank == 'D') {
                        dir_anti_tank = 'R';
                    }
                    else if (dir_anti_tank == 'L') {
                        dir_anti_tank = 'U';
                    }
                    else {
                        //std::cout << "Laser arrete Mirroir UR" << std::endl;
                        verification_Bloc_Mobile(mapParams, &bloc_mobile, partParams);
                        dir_anti_tank = 'X';
                    }
                    break;
                case Rotative_Mirror_UR:
                    if (dir_anti_tank == 'D') {
                        dir_anti_tank = 'R';
                    }
                    else if (dir_anti_tank == 'L') {
                        dir_anti_tank = 'U';
                    }
                    else {
                        //std::cout << "Laser arrete Rotative Mirroir UR" << std::endl;
                        partParams->matrice_mobile[pos_laser_y][pos_laser_x] = Rotative_Mirror_DR;
                        dir_anti_tank = 'X';
                    }
                    break;
                case Bricks:
                    //std::cout << "Laser arrete Bricks" << std::endl;
                    partParams->matrice_mobile[pos_laser_y][pos_laser_x] = Dirt;
                    dir_anti_tank = 'X';
                    break;
                case Anti_Tank_U:
                case Anti_Tank_Dead_U:
                    if (dir_anti_tank == 'D') {
                        partParams->matrice_mobile[pos_laser_y][pos_laser_x] = Anti_Tank_Dead_U;
                    }
                    else {
                        verification_Bloc_Mobile(mapParams, &bloc_mobile, partParams);
                    }
                    //std::cout << "Laser arrete Anti Tank U" << std::endl;
                    dir_anti_tank = 'X';
                    break;
                case Anti_Tank_D:
                case Anti_Tank_Dead_D:
                    if (dir_anti_tank == 'U') {
                        partParams->matrice_mobile[pos_laser_y][pos_laser_x] = Anti_Tank_Dead_D;
                    }
                    else {
                        verification_Bloc_Mobile(mapParams, &bloc_mobile, partParams);
                    }
                    //std::cout << "Laser arrete Anti Tank D" << std::endl;
                    dir_anti_tank = 'X';
                    break;
                case Anti_Tank_L:
                case Anti_Tank_Dead_L:
                    if (dir_anti_tank == 'R') {
                        partParams->matrice_mobile[pos_laser_y][pos_laser_x] = Anti_Tank_Dead_L;
                    }
                    else {
                        verification_Bloc_Mobile(mapParams, &bloc_mobile, partParams);
                    }
                    //std::cout << "Laser arrete Anti Tank L" << std::endl;
                    dir_anti_tank = 'X';
                    break;
                case Anti_Tank_R:
                case Anti_Tank_Dead_R:
                    if (dir_anti_tank == 'L') {
                        partParams->matrice_mobile[pos_laser_y][pos_laser_x] = Anti_Tank_Dead_R;
                    }
                    else {
                        verification_Bloc_Mobile(mapParams, &bloc_mobile, partParams);
                    }
                    //std::cout << "Laser arrete Anti Tank R" << std::endl;
                    dir_anti_tank = 'X';
                    break;
                case Movable_Block:
                    verification_Bloc_Mobile(mapParams, &bloc_mobile, partParams);
                    //std::cout << "Laser arrete Anti Tank Movable Block" << std::endl;
                    dir_anti_tank = 'X';
                    break;
                default:
                    break;
                }
            }
            inverserDirection(&dir_anti_tank);
            Deplacement(&dir_anti_tank, &pos_laser_x, &pos_laser_y);
            inverserDirection(&dir_anti_tank);
        }

        if (dir_anti_tank == 'X') {
            //std::cout << "Laser disparu" << std::endl;
            premiere_case = true;
        }
        else {
            //std::cout << "Laser en vie" << std::endl;
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

/**
 * @brief Fonction de verification de la possibilité d'effectuer le déplacement d'un bloc mobile après un tir
 *
 * @param mapParams Les paramètres de la carte
 * @param mobileParams Les paramètres du déplacement du bloc mobile
 * @param partParams Les paramères de la particule
 */
void verification_Bloc_Mobile(mapStruct* mapParams, mobileStruct* mobileParams, particleStruct* partParams)
{
    mobileParams->Origine_y = mobileParams->depl_y;
    mobileParams->Origine_x = mobileParams->depl_x;
    Deplacement(&(mobileParams->dir), &(mobileParams->depl_x), &(mobileParams->depl_y));

    //Vérification des limites si dépasse pas de mouvement enregistré
    if (mobileParams->depl_x > mapParams->nbr_colonnes - 1) {
        mobileParams->depl_x = mapParams->nbr_colonnes - 1;
    }
    else if (mobileParams->depl_x < 0) {
        mobileParams->depl_x = 0;
    }
    else if (mobileParams->depl_y > mapParams->nbr_lignes - 1) {
        mobileParams->depl_y = mapParams->nbr_lignes;
    }
    else if (mobileParams->depl_y < 0) {
        mobileParams->depl_y = 0;
    }
    //Vérifie une position valide
    else {
        switch (partParams->matrice_mobile[mobileParams->depl_y][mobileParams->depl_x])
        {
        case Dirt:
            switch (mapParams->matrice_fixe[mobileParams->depl_y][mobileParams->depl_x])
            {
            case Dirt :
            case Tank:
            case Base:
            case Way_U:
            case Way_D:
            case Way_R:
            case Way_L:
                partParams->matrice_mobile[mobileParams->depl_y][mobileParams->depl_x] = partParams->matrice_mobile[mobileParams->Origine_y][mobileParams->Origine_x];
                partParams->matrice_mobile[mobileParams->Origine_y][mobileParams->Origine_x] = Dirt;
                premiere_case = true;
                break;
            case Water:
                if (partParams->matrice_mobile[mobileParams->Origine_y][mobileParams->Origine_x] == Movable_Block) {
                    partParams->matrice_mobile[mobileParams->depl_y][mobileParams->depl_x] = Bridge;
                }
                partParams->matrice_mobile[mobileParams->Origine_y][mobileParams->Origine_x] = Dirt;
                break;
            case Ice:
                partParams->matrice_mobile[mobileParams->depl_y][mobileParams->depl_x] = partParams->matrice_mobile[mobileParams->Origine_y][mobileParams->Origine_x];
                partParams->matrice_mobile[mobileParams->Origine_y][mobileParams->Origine_x] = Dirt;
                verification_Bloc_Mobile(mapParams, mobileParams, partParams);
                break;
            case Tunnel_Red:
            case Tunnel_Green:
            case Tunnel_Blue:
            case Tunnel_Cyan:
            case Tunnel_Yellow:
            case Tunnel_Pink:
            case Tunnel_White:
            case Tunnel_Dark:
                portail_Bloc_Mobile(mapParams, mobileParams, partParams);
                break;
            default:
                break;
            }
            break;
        case Bridge:
            partParams->matrice_mobile[mobileParams->depl_y][mobileParams->depl_x] = partParams->matrice_mobile[mobileParams->Origine_y][mobileParams->Origine_x];
            partParams->matrice_mobile[mobileParams->Origine_y][mobileParams->Origine_x] = Dirt;
            premiere_case = true;
            break;
        case Water:
            if (partParams->matrice_mobile[mobileParams->Origine_y][mobileParams->Origine_x] == Movable_Block) {
                partParams->matrice_mobile[mobileParams->depl_y][mobileParams->depl_x] = Bridge;
            }
            partParams->matrice_mobile[mobileParams->Origine_y][mobileParams->Origine_x] = Dirt;
            break;

        case Thin_Ice:
            partParams->matrice_mobile[mobileParams->depl_y][mobileParams->depl_x] = partParams->matrice_mobile[mobileParams->Origine_y][mobileParams->Origine_x];
            partParams->matrice_mobile[mobileParams->Origine_y][mobileParams->Origine_x] = Dirt;
            if (premiere_case) {
                partParams->matrice_mobile[mobileParams->Origine_y][mobileParams->Origine_x] = Dirt;
                premiere_case = false;
            }
            else {
                partParams->matrice_mobile[mobileParams->Origine_y][mobileParams->Origine_x] = Water;
            }
            verification_Bloc_Mobile(mapParams, mobileParams, partParams);
            partParams->matrice_mobile[mobileParams->Origine_y][mobileParams->Origine_x] = Water;
            break;
        default:
            break;
        }
    }
}

/**
 * @brief Gère le déplacement des blocs mobiles par un portail (tunnel)
 *
 * @param mapParams Les paramètres de la carte
 * @param moveParams Les paramètres du déplacement du bloc mobile
 * @param partParams Les paramères de la particule
 */
void portail_Bloc_Mobile(mapStruct* mapParams, mobileStruct* mobileParams, particleStruct* partParams)
{
    int couleur = mapParams->matrice_fixe[mobileParams->depl_y][mobileParams->depl_x];
    bool sortie = false;

    //Parse la carte pour trouver le premier portail correspondant au portail d'entrée mais étant différant.
    for (int i = 0; i < mapParams->nbr_lignes; i++) {
        for (int j = 0; j < mapParams->nbr_colonnes; j++) {
            if (mapParams->matrice_fixe[i][j] == couleur && (i != mobileParams->depl_y || j != mobileParams->depl_x)) {
                if ((partParams->matrice_mobile[i][j] == Dirt)) {
                    partParams->matrice_mobile[i][j] = partParams->matrice_mobile[mobileParams->Origine_y][mobileParams->Origine_x];
                    partParams->matrice_mobile[mobileParams->Origine_y][mobileParams->Origine_x] = Dirt;
                }
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
        partParams->matrice_mobile[mobileParams->Origine_y][mobileParams->Origine_x] = Dirt;
    }
}
