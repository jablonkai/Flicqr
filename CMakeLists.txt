project(Flicqr)

cmake_minimum_required(VERSION 2.8.0)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5WebKitWidgets REQUIRED)

add_subdirectory(src)
