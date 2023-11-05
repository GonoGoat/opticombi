#include "ParticleInit.h"

int main() {
    int numParticles = 50;
    std::vector<Particle> generatedParticles = generateParticles(numParticles);

    for (int i = 0; i < numParticles; i++) {
        std::cout << "Particle " << i + 1 << ": ";
        std::cout << "X: " << generatedParticles[i].x << " ";
        std::cout << "Y: " << generatedParticles[i].y << " ";
        std::cout << "VelocityX: " << generatedParticles[i].velocityX << " ";
        std::cout << "VelocityY: " << generatedParticles[i].velocityY << std::endl;
    }

    return 0;
}