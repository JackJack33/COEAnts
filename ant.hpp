#include <random>
#include <numbers>
#include <cmath>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
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
        Ant(std::shared_ptr<std::default_random_engine> engine_in, CircleObstacle nest, float rad_in) : engine(engine_in), rad(rad_in) {
            x = nest.get_x() + std::cos(rad) * nest.get_r();
            y = nest.get_y() + std::sin(rad) * nest.get_r();
        }
        Ant(std::shared_ptr<std::default_random_engine> engine_in, CircleObstacle nest, float x_in, float y_in, float rad_in) : engine(engine_in), x(x_in), y(y_in), rad(rad_in) {}

        float getX() { return x; }
        float getY() { return y; }
        bool carrying_food() { return has_food; }

        std::vector<std::shared_ptr<Marker>> get_markers_fov(std::vector<std::shared_ptr<Marker>> markers, float speed, int antenna_depth, float fov) {
            auto ant_fov = [this, fov](float x_in, float y_in) -> bool {
                float ang = std::atan((y_in - y) / (x_in - x));
                ang += PI * (ang < 0);
                return std::abs(ang - rad) < fov / 2;
            };
            std::vector<std::shared_ptr<Marker>> markers_in_fov;
            for (auto marker : markers) {
                float marker_dist = marker->distance(x, y);
                if (marker_dist < speed * antenna_depth && marker_dist > 0.05 && marker->in_fov(ant_fov)) {
                    markers_in_fov.push_back(marker);
                }
            }
            return markers_in_fov;
        }

        std::pair<float, float> get_antenna_bias(float offset, CircleObstacle food, CircleObstacle nest, std::vector<std::shared_ptr<Obstacle>> walls, const std::vector<std::shared_ptr<Marker>>& markers_in_fov, float speed, int antenna_depth=4) {
            float offset_rad = rad + offset;
            float next_x = x, next_y = y;

            std::pair<float, float> bias = {0, 0};
            for (int depth = 0; depth < antenna_depth; depth++) {
                next_x += std::cos(offset_rad) * speed;
                next_y += std::sin(offset_rad) * speed;

                for (const auto& marker : markers_in_fov) {
                    if (has_food != marker->is_food()) {
                        // Marker bias = depth * (timespan/10) / distance^2
                        bias.first += (antenna_depth - depth) * marker->get_span() / 10. / std::pow(marker->distance(next_x, next_y), 2);
                    }
                }
                if (markers_in_fov.size() != 0) {
                    bias.first /= markers_in_fov.size();
                }

                // Collision bias = Depth
                if ((!has_food && food.isColliding(next_x, next_y)) || (has_food && nest.isColliding(next_x, next_y))){
                    bias.second += 10. * (antenna_depth - depth) / (speed * speed);
                } else {
                    for (const auto& wall : walls) {
                        if (wall->isColliding(next_x, next_y)) {
                            // Short circuit kill branch
                            return bias;
                        }
                    }
                    bias.second += (antenna_depth - depth) / (speed * speed);
                }
            }
            return bias;
        }

        void move(CircleObstacle food, CircleObstacle nest, std::vector<std::shared_ptr<Obstacle>> walls, const std::vector<std::shared_ptr<Marker>>& markers, float speed, int& nfood, int antenna_depth, float fov, int n_antennas, std::pair<float, float> bias_ratio) {
            float next_x = x + std::cos(rad) * speed, next_y = y + std::sin(rad) * speed;
            if (!has_food && food.isColliding(next_x, next_y)) {
                nfood--;
                std::cout << "An ant has taken food! There are " << nfood << " left!\n";
                x = next_x;
                y = next_y;
                rad += PI;
            } else if (has_food && nest.isColliding(next_x, next_y)) {
                x = next_x;
                y = next_y;
                rad += PI;
            } else {
                std::vector<std::shared_ptr<Marker>> markers_in_fov = get_markers_fov(markers, speed, antenna_depth, fov);
                std::vector<std::pair<float, float>> biases;
                std::vector<float> offsets(n_antennas);
                for (int i = 0; i < n_antennas; i++) {
                    offsets.at(i) = i * fov / (n_antennas - 1) - fov / 2;
                }
                for (int offset_ind = 0; offset_ind < offsets.size(); offset_ind++) {
                    std::pair<float, float> bias = get_antenna_bias(offsets.at(offset_ind), food, nest, walls, markers_in_fov, speed);
                    biases.push_back(bias);
                }
                auto [min_marker, max_marker] = std::minmax_element(biases.begin(), biases.end(), [] (auto& l, auto& r) { return l.first < r.first; });
                auto [min_collision, max_collision] = std::minmax_element(biases.begin(), biases.end(), [] (auto& l, auto& r) { return l.second < r.second; });
                float marker_range = std::max(max_marker->first - min_marker->first, 1.f);
                float collision_range = std::max(max_collision->first - min_collision->first, 1.f);

                std::vector<float> norm_biases;
                float norm_sum = 0;

                for (int i = 0; i < offsets.size(); i++) {
                    float branch_bias = bias_ratio.first * (biases.at(i).first - min_marker->first) / marker_range + bias_ratio.second * (biases.at(i).second - min_collision->second) / collision_range;
                    norm_sum += branch_bias;
                    norm_biases.push_back(branch_bias);
                }

                if (norm_sum < .01) {
                    for (int i = 1; i <= offsets.size(); i++) {
                        norm_biases.at(i - 1) = float(i) / offsets.size();
                    }
                } else {
                    norm_biases.at(0) /= norm_sum;
                    for (int i = 1; i < offsets.size(); i++) {
                        norm_biases.at(i) = norm_biases.at(i - 1) + norm_biases.at(i) / norm_sum;
                    }
                }

                float random_bias_val = std::uniform_real_distribution<float>(0., 1)(*engine);

                int win_branch;
                for (win_branch = 0; win_branch < offsets.size() - 1; win_branch++) {
                    if (random_bias_val < norm_biases.at(win_branch)) {
                        break;
                    }
                }

                rad += offsets.at(win_branch);
                next_x = x + std::cos(rad) * speed;
                next_y = y + std::sin(rad) * speed;
                bool wall_col = false;
                for (auto& wall: walls) {
                    if (wall->isColliding(next_x, next_y)) {
                        wall_col = true;
                        break;
                    }
                }
                if (wall_col) {
                    rad += PI;
                } else {
                    x = next_x;
                    y = next_y;
                }
            }
        }

        void place_marker(std::vector<std::shared_ptr<Marker>>& markers) {
            markers.push_back(std::make_shared<Marker>(x, y, has_food));
        }

};
