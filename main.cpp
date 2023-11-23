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

  vector<Obstacle> obs(1);
  obs.at(0) = test;

  Window window = Window(25,25,obs);
  window.initializeObstacles();
  window.draw();
}
