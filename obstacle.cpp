#include <functional>
#include <cmath>

#include "obstacle.hpp"

bool Obstacle::is_colliding(float x_in, float y_in) {
  float dx = x_in - x; float dy = y_in - y;
  return collisionFunction(dx, dy);
};

//bool Obstacle::is_colliding(Ant ant_in) {
  // needs implementation
//};
