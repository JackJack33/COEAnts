#include <cmath>
#include <vector>
#include <iostream>

#include "visualize.hpp"

using std::cout;

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
	cout << "# ";
	break;
      default:
	cout << "  ";
      }
    }
    cout << "\n";
  }
};
