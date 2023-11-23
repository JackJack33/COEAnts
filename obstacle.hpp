#include <functional>
#include <cmath>
#include "ant.hpp"

using std::function;

enum ObstacleType { AIR, WALL, FOOD, NEST };

class Obstacle {
private:
  float x, y;
  ObstacleType obstacleType;
  // this allows us to define any shape if we want to
  function< bool(float, float) > collisionFunction;
public:
  Obstacle() : x(0.0), y(0.0), obstacleType(WALL) {
    // default is uncollidable object
    auto cf = [](float dx, float dy) -> bool { return false; };
    collisionFunction = cf;
  };

  Obstacle(float x_in, float y_in, ObstacleType ot_in, function< bool(float, float) > cf_in) :
    x(x_in), y(y_in), obstacleType(ot_in), collisionFunction(cf_in) {};

  ObstacleType getObstacleType() { return obstacleType; };

  bool isColliding(float x_in, float y_in);
  //bool is_colliding(Ant ant_in);
};

class CircleObstacle : public Obstacle {
private: 
  float r;
public:
  // default is uncollidable object
  CircleObstacle() : Obstacle() {};
 
  CircleObstacle(float x_in, float y_in, ObstacleType ot_in, float r_in) :
    r(r_in), Obstacle(x_in, y_in, ot_in,
	     [r](float dx, float dy) -> bool { return (dx*dx + dy*dy <= r*r) }) {};
};
  
class RectObstacle : public Obstacle {
public:
  // default is uncollidable object
  RectObstacle() : Obstacle() {};
 
  RectObstacle(float x_in, float y_in, ObstacleType ot_in, float height, float width) :
    Obstacle(x_in, y_in, ot_in,
	     [height,width](float dx, float dy) -> bool { return (std::abs(dx)-width <= 0 && std::abs(dy)-height <= 0) }) {};
};
