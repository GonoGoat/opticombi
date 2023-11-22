#include <iostream>
#include <vector>
#include <random>

struct Particle {
    int x;
    int y;
    int velocityX;
    int velocityY;
};

std::vector<Particle> generateParticles(int numParticles);
