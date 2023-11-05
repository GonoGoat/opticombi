#include <ParticleInit.h>

std::vector<Particle> generateParticles(int numParticles) {
    std::vector<Particle> particles;

    // Générateur de nombres aléatoires
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> x_distribution(0, 15);
    std::uniform_int_distribution<int> y_distribution(0, 15);
    std::uniform_int_distribution<int> velocityX_distribution(-10, 10);
    std::uniform_int_distribution<int> velocityY_distribution(-10, 10);
    
    for (int i = 0; i < numParticles; i++) {
        Particle particle;
        // Générer des coordonnées (x, y) aléatoires entre 0 et 15
        particle.x = x_distribution(gen);
        particle.y = y_distribution(gen);
        // Générer une vitesse aléatoire entre -10 et 10
        particle.velocityX = velocityX_distribution(gen);
        particle.velocityY = velocityY_distribution(gen);
        particles.push_back(particle);
    }

    return particles;
}