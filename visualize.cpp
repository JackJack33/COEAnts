#include <cmath>
#include <vector>
#include <iostream>

#include "visualize.hpp"

using std::cout;
using namespace Color;

void Window::initializeObstacles() {

        obstaclePixels.resize(width, vector<PixelType>(height));

        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {

                if (x == width-1 || x == 0 || y == height-1 || y == 0) {
                    obstaclePixels.at(x).at(y) = PixelType::WALL;
                    continue;
                }

                for (auto& obstacle : obstacles) {
                    if (obstacle->isColliding(x, y)) {
                        obstaclePixels.at(x).at(y) = (PixelType)obstacle->getObstacleType();
                    }
                }
            }
        }
    };

void Window::updateObstaclePixels() {
    for (auto& m : markers) {
        // floats x, y, span
        int vx = std::lround(m->getX()); int vy = std::lround(m->getY());
        if (m->is_food()) { obstaclePixels.at(vx).at(vy) = PixelType::MARKER_NEST; }
        else { obstaclePixels.at(vx).at(vy) = PixelType::MARKER_FOOD; }
    }
    for (auto& a : ants) {
        int vx = std::lround(a->getX()); int vy = std::lround(a->getY());
        if (a->carrying_food()) { obstaclePixels.at(vx).at(vy) = PixelType::ANT_FOOD; }
        else { obstaclePixels.at(vx).at(vy) = PixelType::ANT_NEST; }
    }
}

void Window::draw() {

    updateObstaclePixels();

    for (vector<PixelType> x : obstaclePixels) {
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
                default:
                    cout << " ";
            }
            cout << Paint(FG_DEFAULT) << Paint(BG_DEFAULT) << " ";
        }
        cout << "\n";
    }
};