#include <functional>
#include <vector>
#include <cmath>

#include "obstacle.hpp";

using std::vector;

class Window {
private:
  uint width; uint height;
  vector<Obstacle> obstacles;
  vector<vector<uint>> pixels;

public:

  Window() : width(10), height(10) {};
  
  Window(uint w_in, uint h_in) : width(w_in), height(h_in) {};

  Window(uint w_in, uint h_i, vector<Obstacle> o_in) : width(w_in), height(h_in), obstacles(o_in) {};

  void initializeObstacles();
  
  void draw();
}
