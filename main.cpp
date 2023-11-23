#include <vector>
#include <functional>
#include "visualize.hpp"

using std::function;
using std::vector;

int main() {
  Obstacle test = Obstacle(10.0, 10.0, WALL,
			   [](float dx, float dy) -> bool {
			     if (dx*dx + dy*dy <= 20) { return true; }
			     return false;
			   });

  Obstacle test2 = Obstacle(5.0, 5.0, FOOD,
			    [](float dx, float dy) -> bool {
			      if (std::abs(dx) <= 2.5 && std::abs(dy) <= 1) { return true; }
			      return false;
			    });

  Obstacle test3 = Obstacle(20.0, 20.0, NEST,
			    [](float dx, float dy) -> bool {
			     if (dx*dx + dy*dy <= 4) { return true; }
			     return false;
			   });

  vector<Obstacle> obs(3);
  obs.at(0) = test;
  obs.at(1) = test2;
  obs.at(2) = test3;

  Window window = Window(25,25,obs);
  window.initializeObstacles();
  window.draw();
}
