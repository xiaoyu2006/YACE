cmake_minimum_required(VERSION 3.5)

project(YACE LANGUAGES C CXX)

# Program

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_subdirectory(cchip8)

set(PROJECT_SOURCES
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

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QChip8
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QChip8 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QChip8 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QChip8
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(QChip8 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_include_directories(QChip8 PUBLIC ${CCHIP8_INCLUDE})
target_link_libraries(QChip8 PUBLIC CChip8)
set_target_properties(QChip8 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER top.ycao.qchip8
    MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS QChip8
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QChip8)
endif()

# Testing
enable_testing()
find_package(Qt6Test REQUIRED)
add_executable(CPUTests cputests.cpp)
target_include_directories(CPUTests PUBLIC ${CCHIP8_INCLUDE})
target_link_libraries(CPUTests CChip8 Qt6::Test)
add_test(CPUTests CPUTests)

