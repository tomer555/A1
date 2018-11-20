cmake_minimum_required(VERSION 3.12)
project(A1)

set(CMAKE_CXX_STANDARD 11)

add_executable(rest src/Main.cpp src/Restaurant.cpp src/Dish.cpp src/Customer.cpp src/Action.cpp src/Table.cpp)