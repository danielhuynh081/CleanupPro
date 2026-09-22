cmake_minimum_required(VERSION 3.16)
project(CleanupPro LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Functions 
add_library(cleanuppro_core STATIC src/core/core.cpp)
target_include_directories(cleanuppro_core PUBLIC include)
target_compile_options(cleanuppro_core PRIVATE
    -Wall -Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations
    -Wextra -Wpedantic -Werror)

# CLI app
add_executable(cleanuppro src/cli/main.cpp)
target_link_libraries(cleanuppro PRIVATE cleanuppro_core)
target_compile_options(cleanuppro PRIVATE
    -Wall -Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations
    -Wextra -Wpedantic -Werror)

# GUI app
find_package(Qt6 COMPONENTS Widgets Concurrent QUIET)
if(Qt6_FOUND)
    add_executable(cleanuppro_gui src/gui/gui.cpp src/gui/gui.h)
    target_link_libraries(cleanuppro_gui PRIVATE cleanuppro_core Qt6::Widgets Qt6::Concurrent)
else()
    message(STATUS "Qt6 not found - skipping GUI target")
endif()
