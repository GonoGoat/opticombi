#ifndef DATA_MATRIX
#define DATA_MATRIX

enum Matrice
{
    Dirt,
    Tank,
    Base,
    Water,
    Sollid_Block,
    Movable_Block,
    Bricks,
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

std::unordered_map<std::string, Matrice> conversionToEnum = {
    {"D",Dirt},{"Tu",Tank},{"b",Base},{"w",Water},{"Bs",Sollid_Block},
    {"Bm",Movable_Block},{"B",Bricks},{"Au",Anti_Tank_U},{"Ad",Anti_Tank_D},
    {"Al",Anti_Tank_L},{"Ar",Anti_Tank_R},{"Mur",Mirror_UR},{"Mul",Mirror_UL},
    {"Mdr",Mirror_DR},{"Mdl",Mirro_DL},{"Wu",Way_U},{"Wd",Way_D},{"Wr",Way_R},
    {"Wl",Way_L},{"C",Crystal_Block},{"Rur",Rotative_Mirror_UR},{"Rul",Rotative_Mirror_UL},
    {"Rdr",Rotative_Mirror_DR},{"Rdl",Rotative_Mirror_DL},{"I",Ice},{"i",Thin_Ice},
    {"Tr",Tunnel_Red},{"Tg",Tunnel_Green},{"Tb",Tunnel_Blue},{"Tc",Tunnel_Cyan},
    {"Ty",Tunnel_Yellow},{"Tp",Tunnel_Pink},{"Tw",Tunnel_White},{"Td",Tunnel_Dark}
};

#endif