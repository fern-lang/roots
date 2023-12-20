cmake_minimum_required(VERSION 3.19.6)

project(Roots CXX)

# Allow for forced colored output
option(ROOTS_FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." ON)
if(${FORCED_COLORED_OUTPUT})
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always)
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-fcolor-diagnostics)
  endif()
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(roots
  STATIC
  lib/Environment.cpp
  lib/Error.cpp
  lib/Filesystem.cpp
  lib/Memory.cpp
  lib/String.cpp
)

add_library(Roots::Roots ALIAS roots)

option(ROOTS_DEBUG "Enable memory debugging." OFF)


if(${ROOTS_DEBUG} OR ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  target_compile_definitions(roots
    PUBLIC
    RootsDebug
  )
endif()

target_include_directories(roots
  PUBLIC
  include
)
