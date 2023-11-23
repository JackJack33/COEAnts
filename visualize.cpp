#include <cmath>
#include <vector>
#include <iostream>

#include "visualize.hpp"

using std::cout;
using namespace Color;

void Window::initializeObstacles() {

  obstaclePixels.resize(width, vector<ObstacleType>(height));

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      for (Obstacle obstacle : obstacles) {
	if (obstacle.isColliding(x, y)) {
	  obstaclePixels.at(x).at(y) = obstacle.getObstacleType();
	}
      }
    }
  }
};

void Window::draw() {
  for (vector<ObstacleType> x : obstaclePixels) {
    for (ObstacleType y : x) {
      switch (y) {
      case WALL:
	cout << Paint(BG_RED) << Paint(FG_BLUE) << "#";
	break;
      default:
	cout << " ";
      }
      cout << Paint(FG_DEFAULT) << Paint(BG_DEFAULT) << " ";
    }
    cout << "\n";
  }
};
