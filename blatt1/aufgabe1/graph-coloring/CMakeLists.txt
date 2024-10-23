cmake_minimum_required(VERSION 3.22)
project(graph_coloring)

set(CMAKE_CXX_STANDARD 11)

# Add include directory for external library
include_directories(${CMAKE_SOURCE_DIR}/third_party/cadical/include)

# Add the library path for linking
link_directories(${CMAKE_SOURCE_DIR}/third_party/cadical/lib)

add_executable(graph_coloring main.cpp)

# Link the external static library
target_link_libraries(graph_coloring cadical)
