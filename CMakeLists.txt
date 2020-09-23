cmake_minimum_required(VERSION 3.5)

project(QChip8 LANGUAGES C CXX)

# Program

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_subdirectory(cchip8)

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
)

target_include_directories(QChip8 PUBLIC ${CCHIP8_INCLUDE})

target_link_libraries(QChip8 PUBLIC CChip8)
target_link_libraries(QChip8 PRIVATE Qt5::Widgets)


# Testing
enable_testing()

find_package(Qt5Test REQUIRED)
add_executable(CPUTests
 cputests.cpp
)
target_include_directories(CPUTests PUBLIC ${CCHIP8_INCLUDE})
target_link_libraries(CPUTests CChip8 Qt5::Test)

add_test(CPUTests CPUTests)
