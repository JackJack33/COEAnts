#include <vector>
#include <functional>
#include <unistd.h>
#include "memory"
#include "visualize.hpp"


using std::function;
using std::vector;

int main() {
    std::shared_ptr<std::default_random_engine> engine = std::make_shared<std::default_random_engine>();
    engine->discard(1);
    int nfood = 5;
    int nants = 10;
    int nantennas = 3;
    int antenna_depth = 4;
    float speed = 1;
    float fov = 3.1415 / 6;
    std::pair<float, float> bias_ratio = {.34, .66};
    vector<std::shared_ptr<Obstacle>> walls = {std::make_shared<Obstacle>(8, 4, ObstacleType(WALL), 2, 5),
                                               std::make_shared<Obstacle>(-5, -5, ObstacleType(WALL), 6, 35),
                                               std::make_shared<Obstacle>(0, 25, ObstacleType(WALL), 30, 5),
                                               std::make_shared<Obstacle>(25, -5, ObstacleType(WALL), 5, 30),
                                               std::make_shared<Obstacle>(0, -5, ObstacleType(WALL), 26, 5)};
    CircleObstacle nest = CircleObstacle(5, 5, ObstacleType(NEST), .5);
    CircleObstacle food = CircleObstacle(15, 15, ObstacleType(FOOD), 3);
    vector<std::shared_ptr<Marker>> markers;

    vector<std::shared_ptr<Ant>> ants;
    for (int i = 0; i < nants; i++) {
        ants.push_back(std::make_shared<Ant>(engine, nest));
    }

    Window window = Window(26, 26, nest, food, walls, ants);
    window.initializeObstacles();

    int frames_elapsed = 0;
    while (nfood > 0) {
        frames_elapsed++;
        for (auto ant: ants) {
            if (frames_elapsed % 3 == 0) {
                ant->place_marker(markers);
            }
            ant->move(food, nest, walls, markers, speed, nfood, antenna_depth, fov, nantennas, bias_ratio);
        }
        for (int i = 0; i < markers.size(); i++) {
            markers.at(i)->decrement_span();
        }
        markers.erase(std::remove_if(std::begin(markers), std::end(markers), [](std::shared_ptr<Marker> m) -> bool { return m->get_span() == 0; }), markers.end());
        window.draw(markers);
        std::cout << "Frames Elapsed: " << frames_elapsed << '\n';
        usleep(1000000);
    }
}
