#include <vector>
#include <functional>
#include <memory>
#include <iostream>

#include "visualize.hpp"


using std::function;
using std::vector;

int main() {
    int nants = 10;

    CircleObstacle nest = CircleObstacle(5, 5, ObstacleType::NEST, 2);
    CircleObstacle food = CircleObstacle(15, 15, ObstacleType::FOOD, 3);
    int nfood = 30;

    vector<std::shared_ptr<Obstacle>> walls = {std::make_shared<Obstacle>(8, 5, ObstacleType::WALL, 2, 4)};
    vector<Obstacle> walls2 = {Obstacle(8, 5, ObstacleType::WALL, 2, 4),
			       Obstacle(8, 18, ObstacleType::WALL, 5, 5),
			       Obstacle(2, 2, ObstacleType::NEST, 4, 4)};
    

    std::shared_ptr<std::default_random_engine> engine = std::make_shared<std::default_random_engine>();
    //vector<std::shared_ptr<Ant>> ants;
    //for (int i = 0; i < nants; i++) {
    //    ants.push_back(std::make_shared<Ant>(engine, nest));
    //}

    vector<Ant> ants;
    for (int i = 0; i < nants; i++) {
      ants.push_back(Ant(engine, nest));
    }

    vector<Marker> markers = {Marker(10, 10, true),
			      Marker(1, 1, false),
			      Marker(1, 3, true),
			      Marker(2, 4)};


    int frames_elapsed = 0;
    while (nfood > 0) {
        frames_elapsed++;
        for (auto ant: ants) {
            if (frames_elapsed % 3 == 0) {
                ant->place_marker(markers);
            }
            ant->move(food, nest, walls, markers, 3, nfood);
        }
        for (auto marker: markers) {
            marker->decrement_span();
        }

    }


    Window window = Window(25, 25, walls2, &markers);
    window.initializeObstacles();
    window.draw();
}
