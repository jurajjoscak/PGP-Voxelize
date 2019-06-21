cmake_minimum_required(VERSION 3.7)
project(PGP-Voxelize)

include_directories(.)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(SDL2Test ${SDL2_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})

configure_file(VERTEX_SHADER.gls VERTEX_SHADER.gls COPYONLY)
configure_file(COMPUTE_SHADER.gls COMPUTE_SHADER.gls COPYONLY)
configure_file(FRAGMENT_SHADER.gls FRAGMENT_SHADER.gls COPYONLY)
configure_file(VERTEX_SHADER_VOXEL.gls VERTEX_SHADER_VOXEL.gls COPYONLY)
configure_file(FRAGMENT_SHADER_VOXEL.gls FRAGMENT_SHADER_VOXEL.gls COPYONLY)
configure_file(GEOMETRY_SHADER_VOXEL.gls GEOMETRY_SHADER_VOXEL.gls COPYONLY)


add_executable(PGP-Voxelize main.cpp model.cpp model.h)

target_link_libraries(PGP-Voxelize ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})