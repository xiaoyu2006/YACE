cmake_minimum_required(VERSION 3.5)

project(QChip8 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(QChip8
  main.cpp

  onboardwindow.h
  onboardwindow.cpp
  onboardwindow.ui

  chip8ui.h
  chip8ui.cpp
  chip8ui.ui

  chip8screen.h
  chip8screen.cpp

  keymap.h
  keymap.cpp

  cchip8/cpu.h
  cchip8/cpu.c
  cchip8/instructionset.h
  cchip8/instructionset.c
  cchip8/interface.h
  cchip8/interface.c
)

target_link_libraries(QChip8 PRIVATE Qt5::Widgets)
