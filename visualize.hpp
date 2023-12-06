#include <functional>
#include <vector>
#include <cmath>
#include <iostream>
#include <memory>

#include "ant.hpp"

using std::vector;

// no extended enums in c++ :( implementation sucks but works
enum class PixelType {AIR, WALL, FOOD, NEST, MARKER_FOOD, MARKER_NEST, ANT_FOOD, ANT_NEST};

class Window {
private:
  uint width; uint height;
  CircleObstacle nest;
  CircleObstacle food;
  vector<std::shared_ptr<Obstacle>> obstacles;
  vector<std::shared_ptr<Ant>> ants;
  vector<std::shared_ptr<Marker>> markers;
  vector<vector<PixelType>> obstaclePixels;

public:

  Window() : width(10), height(10), nest(CircleObstacle(0,0,ObstacleType(NEST),1)), food(CircleObstacle(5,5,ObstacleType(FOOD),1))  {};
  
  Window(uint w_in, uint h_in) : width(w_in), height(h_in), nest(CircleObstacle(0,0,ObstacleType(NEST),1)), food(CircleObstacle(5,5,ObstacleType(FOOD),1)) {};

  Window(uint w_in, uint h_in, CircleObstacle nest_in, CircleObstacle food_in, vector<std::shared_ptr<Obstacle>> o_in, vector<std::shared_ptr<Ant>> ants_in, vector<std::shared_ptr<Marker>> markers_in)
        : width(w_in), height(h_in), food(food_in), nest(nest_in), obstacles(o_in), ants(ants_in), markers(markers_in) {};

  void initializeObstacles();

    void updateObstaclePixels();

    void draw();
};

namespace Color {
  enum Code {
    FG_DEFAULT = 39,
    BG_DEFAULT = 49,

    FG_BLACK = 30,
    FG_RED = 31,
    FG_GREEN = 32,
    FG_YELLOW = 33,
    FG_BLUE = 34,
    FG_MAGENTA = 35,
    FG_CYAN = 36,
    FG_WHITE = 37,

    FG_BRIGHT_BLACK = 90,
    FG_BRIGHT_RED = 91,
    FG_BRIGHT_GREEN = 92,
    FG_BRIGHT_YELLOW = 93,
    FG_BRIGHT_BLUE = 94,
    FG_BRIGHT_MAGENTA = 95,
    FG_BRIGHT_CYAN = 96,
    FG_BRIGHT_WHITE = 97,

    BG_BLACK = 40,
    BG_RED = 41,
    BG_GREEN = 42,
    BG_YELLOW = 43,
    BG_BLUE = 44,
    BG_MAGENTA = 45,
    BG_CYAN = 46,
    BG_WHITE = 47,

    BG_BRIGHT_BLACK = 100,
    BG_BRIGHT_RED = 101,
    BG_BRIGHT_GREEN = 102,
    BG_BRIGHT_YELLOW = 103,
    BG_BRIGHT_BLUE = 104,
    BG_BRIGHT_MAGENTA = 105,
    BG_BRIGHT_CYAN = 106,
    BG_BRIGHT_WHITE = 107,    
  };

  class Paint {
  private:
    Code code;
  public:

    Paint() : code(FG_DEFAULT) {};
    
    Paint(Code c_in) : code(c_in) {};

    friend std::ostream& operator<<(std::ostream& os, const Paint p) {
      return os << "\033[" << p.code << "m";
    }

  };
}

