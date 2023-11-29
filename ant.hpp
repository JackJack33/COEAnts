#include <random>
#include <numbers>
#include <cmath>
#include <vector>
#include <memory>
#include "obstacle.hpp"
#include "marker.hpp"

class Ant {
    private:
        const float PI = 3.1415;
        float x, y, ang;
        bool has_food = false;
        std::shared_ptr<std::default_random_engine> engine;
    public:
        Ant(std::shared_ptr<std::default_random_engine> engine_in, CircleObstacle nest) : engine(engine_in) {
            ang = std::uniform_real_distribution<float>(0., 2 * PI)(*engine_in);
            x = nest.get_x() + std::cos(ang) * nest.get_r();
            y = nest.get_y() + std::sin(ang) * nest.get_r();
        }

        void move(CircleObstacle food, CircleObstacle nest, std::vector<std::shared_ptr<Obstacle>> walls, std::vector<std::shared_ptr<Marker>> markers, float speed) {
            ang += std::uniform_real_distribution(-PI/12, PI/12)(*engine);


            if (food.isColliding(x, y)) {
                has_food = true;
                ang += PI;

            } else if (nest.isColliding(x, y)) {
                has_food = false;
                ang += PI;

            } else {
                int i = 0;
                while (i < walls.size() && !walls.at(i)->isColliding(x + std::cos(ang) * speed, y + std::sin(ang) * speed)) {
                    i++;
                }
                if (i != walls.size()) {

                }
            }

            x += std::cos(ang) * speed;
            y += std::sin(ang) * speed;
        }
};