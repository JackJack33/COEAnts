cmake_minimum_required( VERSION 3.12)
project( ants VERSION 1.0 )
add_executable( ants ant.hpp ant.cpp marker.hpp obstacle.hpp obstacle.cpp visualize.hpp visualize.cpp main.cpp )
