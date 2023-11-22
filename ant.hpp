#include <random>
#include <numbers>
#include <cmath>
#include "obstacle.hpp"

class Ant {
    private:
        float x, y, ang;
        bool has_food = false;
    public:
        Ant(default_random_engine engine, CircleObstacle nest) {
            ang = std::uniform_real_distribution(0., 2 * std::numbers::pi)(engine);
            x = nest.x + std::cos(ang) * nest.r;
            y = nest.y + std::sin(ang) * nest.r;
        }

        void move(vector<Obstacle> obstacles, vector<Marker> markers, float speed, default_random_engine engine) {
            ang += std::uniform_real_distribution(-15., 15.)(engine);
            x += std::cos(ang) * speed;
            y += std::sin(ang) * speed;
        }
}