#include <random>
#include <cmath>
#include <vector>
#include <memory>
#include <algorithm>

#include "obstacle.hpp"
#include "marker.hpp"

class Ant {
    private:
        const float PI = 3.1415;
        float x, y, rad;
        bool has_food = false;
        std::shared_ptr<std::default_random_engine> engine;
    public:
        Ant(std::shared_ptr<std::default_random_engine> engine_in, CircleObstacle nest) : engine(engine_in) {
            rad = std::uniform_real_distribution<float>(0., 2 * PI)(*engine_in);
            x = nest.get_x() + std::cos(rad) * nest.get_r();
            y = nest.get_y() + std::sin(rad) * nest.get_r();
        }

  bool carrying_food() { return has_food; }
  float getX() { return x; }
  float getY() { return y; }
  
        std::vector<std::shared_ptr<Marker>> get_markers_fov(std::vector<std::shared_ptr<Marker>> markers, float speed, int antenna_depth, float fov) {
            auto marker_fov = [this, fov](float x_in, float y_in) -> bool {
                float ang = std::atan((x - x_in) / (y - y_in));
                return ang > rad - fov / 2 && ang < rad + fov / 2;
            };
            std::vector<std::shared_ptr<Marker>> markers_in_fov;
            for (auto marker : markers) {
                float marker_dist = marker->distance(x, y);
                if ((marker_dist < speed * antenna_depth && marker_dist > 0.05) || !marker->in_fov(marker_fov)) {
                    markers_in_fov.push_back(marker);
                }
            }
            return markers_in_fov;
        }

        float get_antenna_bias(CircleObstacle food, CircleObstacle nest, std::vector<std::shared_ptr<Obstacle>> walls, std::vector<std::shared_ptr<Marker>> markers, float speed, int antenna_depth=4) {
            std::vector<std::shared_ptr<Marker>> markers_in_fov = get_markers_fov(markers, speed, antenna_depth, PI/6);

            std::vector<float> offsets = {0, -PI / 12, PI / 12};
            for (auto offset : offsets) {
                float offset_rad = rad + offset;
                float next_x = x, next_y = y;

                bool no_walls = true;
                float collision_bias = 0, marker_bias = 0;
                for (int depth = antenna_depth; depth > 0; depth--) {
                    next_x += std::cos(offset_rad) * speed;
                    next_y += std::sin(offset_rad) * speed;

                    for (const auto& marker : markers_in_fov) {
                        if (has_food == marker->is_food()) {
                            marker_bias += marker->get_span() / 10. / std::pow(marker->distance(next_x, next_y), 2);
                        }
                    }
                    marker_bias /= markers_in_fov.size();

                    if ((!has_food && food.isColliding(next_x, next_y)) || (has_food && nest.isColliding(next_x, next_y))){
                        collision_bias +=  10. * depth / (speed * speed) / std::pow(depth + 1 - antenna_depth, 2);
                    } else if (no_walls) {
                        for (const auto& wall : walls) {
                            if (wall->isColliding(next_x, next_y)) { no_walls = false; break; }
                            collision_bias += depth / (speed * speed) / std::pow(depth + 1 - antenna_depth, 2);
                        }
                    }
                }
            }
        }

        void move(CircleObstacle food, CircleObstacle nest, std::vector<std::shared_ptr<Obstacle>> walls, const std::vector<std::shared_ptr<Marker>>& markers, float speed, int& nfood) {
            float next_x = x + std::cos(rad) * speed, next_y = y + std::sin(rad) * speed;
            if (!has_food && food.isColliding(next_x, next_y)) {
                nfood--;
                x = next_x;
                y = next_y;
                rad += PI;
            } else if (has_food && nest.isColliding(next_x, next_y)) {
                x = next_x;
                y = next_y;
                rad += PI;
            } else {
                float bias = get_antenna_bias(food, nest, walls, markers, speed);
            }
        }

        void place_marker(std::vector<std::shared_ptr<Marker>>& markers) {
            markers.push_back(std::make_shared<Marker>(x, y, has_food));
        }

};
//https://www.desmos.com/calculator/xrvjqwuf5m
//Multiple antennas, weight by distance from antenna, check if each antenna is colliding; for markers determine distance to marker
