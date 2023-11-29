#include <functional>
#include <cmath>

using std::function;

enum ObstacleType { AIR, WALL, FOOD, NEST };

class Obstacle {
    private:
        float x, y;
        ObstacleType obstacleType;
        // this allows us to define any shape if we want to
        function< bool(float, float) > collisionFunction;
    public:
        Obstacle(float x_in, float y_in, ObstacleType ot_in, function< bool(float, float) > cf_in) :
                 x(x_in), y(y_in), obstacleType(ot_in), collisionFunction(cf_in) {};
        Obstacle(float x_in, float y_in, ObstacleType ot_in, float width, float height) :
                 x(x_in), y(y_in), obstacleType(ot_in),
                 collisionFunction([width, height](float dx, float dy) -> bool { return dx > 0 && dx < width && dy > 0 && dy < height; }) {}

        ObstacleType getObstacleType() { return obstacleType; };

        float get_x() { return x; }
        float get_y() { return y; }
        bool isColliding(float x_in, float y_in);
};

class CircleObstacle : public Obstacle {
    private:
        float r;
    public:
        CircleObstacle(float x_in, float y_in, ObstacleType ot_in, float r_in) :
                r(r_in), Obstacle(x_in, y_in, ot_in,
                                  [r_in](float dx, float dy) -> bool { return (dx*dx + dy*dy <= r_in*r_in); }) {};
        float get_r() { return r; }
};

// Replaced by Obstacle from width and height constructor
//class RectObstacle : public Obstacle {
//    public:
//        RectObstacle(float x_in, float y_in, ObstacleType ot_in, float h_in, float w_in) :
//                Obstacle(x_in, y_in, ot_in,[h_in, w_in](float dx, float dy) -> bool { return (std::abs(dx)-w_in <= 0 && std::abs(dy)-h_in <= 0); }) {};
//};