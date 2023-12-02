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
    vector<std::shared_ptr<Ant>> ants;
    for (int i = 0; i < nants; i++) {
        ants.push_back(std::make_shared<Ant>(engine, nest));
    }

    vector<Marker> markers = {Marker(10, 10, true),
			      Marker(1, 1, false),
			      Marker(1, 3, true),
			      Marker(2, 4)};

    //while (nfood > 0) {
    //    for (auto ant: ants) {
    //        ant->move(food, nest, walls, markers, 3);
    //    }
    //}

//    Obstacle test = Obstacle(10.0, 10.0, WALL,
//                             [](float dx, float dy) -> bool {
//                                 if (dx * dx + dy * dy <= 20) { return true; }
//                                 return false;
//                             });
//
//    Obstacle test2 = Obstacle(5.0, 5.0, FOOD,
//                              [](float dx, float dy) -> bool {
//                                  if (std::abs(dx) <= 2.5 && std::abs(dy) <= 1) { return true; }
//                                  return false;
//                              });
//
//    Obstacle test3 = Obstacle(20.0, 20.0, NEST,
//                              [](float dx, float dy) -> bool {
//                                  if (dx * dx + dy * dy <= 4) { return true; }
//                                  return false;
//                              });
//
//    vector<Obstacle> obs(3);
//    obs.at(0) = test;
//    obs.at(1) = test2;
//    obs.at(2) = test3;

    Window window = Window(25, 25, walls2, &markers);
    window.initializeObstacles();
    window.draw();
}
