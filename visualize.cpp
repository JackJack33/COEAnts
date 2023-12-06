#include <cmath>
#include <vector>
#include <iostream>

#include "visualize.hpp"

using std::cout;
using namespace Color;

void Window::initializeObstacles() {

        obstaclePixels.resize(width, vector<PixelType>(height));
        pixels.resize(width, vector<PixelType>(height));
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                for (auto& obstacle : obstacles) {
                    if (obstacle->isColliding(x, y)) {
                        obstaclePixels.at(x).at(y) = (PixelType)obstacle->getObstacleType();
                    }
                }
		if (nest.isColliding(x,y)) { obstaclePixels.at(x).at(y) = (PixelType)nest.getObstacleType(); }
		if (food.isColliding(x,y)) { obstaclePixels.at(x).at(y) = (PixelType)food.getObstacleType(); }
            }
        }
    };

void Window::updateObstaclePixels(vector<std::shared_ptr<Marker>>& markers) {
    pixels = obstaclePixels;
    for (auto& m : markers) {
        // floats x, y, span
        int vx = m->getX(); int vy = m->getY();
        if (m->is_food()) { pixels.at(vx).at(vy) = PixelType::MARKER_FOOD; }
        else { pixels.at(vx).at(vy) = PixelType::MARKER_NEST; }
    }

    for (auto& a : ants) {

        int vx = std::lround(a->getX()); int vy = std::lround(a->getY());
        if (a->carrying_food()) { pixels.at(vx).at(vy) = PixelType::ANT_FOOD; }
        else { pixels.at(vx).at(vy) = PixelType::ANT_NEST; }
    }
}

void Window::draw(vector<std::shared_ptr<Marker>>& markers) {

    updateObstaclePixels(markers);

    for (vector<PixelType> x : pixels) {
        for (PixelType y : x) {
            switch (y) {
                case PixelType::WALL:
                    cout << Paint(BG_RED) << Paint(FG_BLACK) << "W";
                    break;
                case PixelType::FOOD:
                    cout << Paint(BG_GREEN) << Paint(FG_BLACK) << "F";
                    break;
                case PixelType::NEST:
                    cout << Paint(BG_BLUE) << Paint(FG_BLACK) << "N";
                    break;
                case PixelType::MARKER_FOOD:
                    cout << Paint(BG_BLACK) << Paint(FG_BLUE) << "F";
                    break;
                case PixelType::MARKER_NEST:
                    cout << Paint(BG_BLACK) << Paint(FG_GREEN) << "N";
                    break;
                case PixelType::ANT_NEST:
                    cout << Paint(BG_YELLOW) << Paint(FG_GREEN) << "A";
                    break;
                case PixelType::ANT_FOOD:
		    cout << Paint(BG_YELLOW) << Paint(FG_BLUE) << "Z";
                    break;
                default:
                    cout << " ";
            }
            cout << Paint(FG_DEFAULT) << Paint(BG_DEFAULT) << " ";
        }
        cout << "\n";
    }
};
