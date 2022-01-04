cmake_minimum_required(VERSION 3.21)
project(mouse_click)

set(CMAKE_CXX_STANDARD 14)

add_executable(mouse_click resources.cpp main.cpp main.h util/Mouse.cpp util/Mouse.h thread/ClickThread.cpp thread/ClickThread.h thread/HotKeyThread.cpp thread/HotKeyThread.h)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})
target_link_libraries(mouse_click ${GTK3_LIBRARIES})
target_link_libraries(mouse_click log4cpp)
target_link_libraries(mouse_click pthread)
target_link_libraries(mouse_click X11)
target_link_libraries(mouse_click Xtst)