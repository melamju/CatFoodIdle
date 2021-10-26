cmake_minimum_required(VERSION 3.20)
project(CatFoodIdle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH /usr/local/lib/cmake)
set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/glfw3)


include_directories(/usr/local/include glad)
include_directories(${GLFW_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp game.cpp game.h shader.cpp shader.h texture.cpp texture.h resource_manager.cpp resource_manager.h stb_image.cpp stb_image.h glad.c)
add_executable(CatFoodIdle ${SOURCE_FILES})

target_link_libraries(CatFoodIdle -lglfw3 -lglu32 -lOpenGL32 -lfreeGLUT -lglut)
