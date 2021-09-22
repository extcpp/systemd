cmake_minimum_required(VERSION 3.14)
project(ext-systemd VERSION 0.0.1 LANGUAGES CXX)

add_executable(ext-systemd-example-daemon daemon.cpp)
target_link_libraries(ext-systemd-example-daemon PUBLIC ext::systemd)
