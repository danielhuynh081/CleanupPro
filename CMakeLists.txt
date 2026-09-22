cmake_minimum_required(VERSION 3.16)
project(CleanupPro LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Functions 
add_library(cleanuppro_functions STATIC src/functions/functions.cpp)
target_include_directories(cleanuppro_functions PUBLIC include)
target_compile_options(cleanuppro_functions PRIVATE
    -Wall -Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations
    -Wextra -Wpedantic -Werror)

# CLI app
add_executable(cleanuppro src/cli/main.cpp)
target_link_libraries(cleanuppro PRIVATE cleanuppro_functions)
target_compile_options(cleanuppro PRIVATE
    -Wall -Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations
    -Wextra -Wpedantic -Werror)
# GUI app
find_package(Qt6 COMPONENTS Widgets Concurrent QUIET)

if(Qt6_FOUND)
    add_executable(CleanupProGUI MACOSX_BUNDLE 
        src/gui/gui.cpp 
        src/gui/gui.h
    )

    # macOS app information
    set_target_properties(CleanupProGUI PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME "CleanupPro"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.danielhuynh.cleanuppro"
        MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    )

    target_link_libraries(CleanupProGUI PRIVATE 
        cleanuppro_functions
        Qt6::Widgets 
        Qt6::Concurrent
    )
else()
    message(STATUS "Qt6 not found - skipping GUI target")
endif()

# C