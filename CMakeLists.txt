# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set (CMAKE_CXX_STANDARD 20)

project ("2048")

add_compile_options(-Werror -Wunused-variable -Wunused-function)

# Include sub-projects.
add_subdirectory ("2048")
