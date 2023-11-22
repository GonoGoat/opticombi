/*#include "ParticleInit.cpp"
#include "deplacement.cpp"

int main() {
    int numParticles = 1;
    std::vector<Particle> generatedParticles = generateParticles(numParticles);

    for (int i = 0; i < numParticles; i++) {
        std::cout << "Particle " << i + 1 << ": ";
        std::cout << "X: " << generatedParticles[i].x << " ";
        std::cout << "Y: " << generatedParticles[i].y << " ";
        std::cout << "VelocityX: " << generatedParticles[i].velocityX << " ";
        std::cout << "VelocityY: " << generatedParticles[i].velocityY << std::endl;
    }

    Deplacement(generatedParticles[0].x,generatedParticles[0].y,5,8,"U");

    return 0;
}*/
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <random>

const int gridSize = 5;

class LaserTankGame {
public:
    LaserTankGame() {
        grid = std::vector<std::vector<char>>(gridSize, std::vector<char>(gridSize, '.')); // Grille du jeu
        tank = std::make_pair(0, 0);
        flag = std::make_pair(gridSize - 1, gridSize - 1);
        is_game_over = false;
    }

    bool is_valid_move(int x, int y) {
        return x >= 0 && x < gridSize && y >= 0 && y < gridSize && grid[y][x] != 'X';
    }

    void move_tank(int dx, int dy) {
        if (is_game_over) return;

        int new_x = tank.first + dx;
        int new_y = tank.second + dy;

        if (is_valid_move(new_x, new_y)) {
            grid[tank.second][tank.first] = '.';
            tank = std::make_pair(new_x, new_y);
            grid[new_y][new_x] = 'T';

            if (tank == flag) {
                is_game_over = true;
            }
        }
    }

    bool game_over() {
        return is_game_over;
    }

    std::pair<int, int> get_flag_position() const {
        return flag;
    }

    void display() {
        for (int i = 0; i < gridSize; ++i) {
            for (int j = 0; j < gridSize; ++j) {
                std::cout << grid[i][j] << ' ';
            }
            std::cout << std::endl;
        }
    }

private:
    std::vector<std::vector<char>> grid;
    std::pair<int, int> tank;
    std::pair<int, int> flag;
    bool is_game_over;
};

// Le reste du code reste inchangé...
