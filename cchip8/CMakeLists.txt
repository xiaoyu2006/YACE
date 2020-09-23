cmake_minimum_required(VERSION 3.0)

project(CChip8 LANGUAGES C)

set(CCHIP8_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)

set(CCHIP8_SOURCES
  cpu.h
  cpu.c
  instructionset.h
  instructionset.c
  interface.h
  interface.c
  errors.h
)

add_library(CChip8 SHARED ${CCHIP8_SOURCES})

target_include_directories(CChip8 PUBLIC ${CCHIP8_INCLUDE})
