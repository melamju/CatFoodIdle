cmake_minimum_required(VERSION 3.20)
project(CatFoodIdle)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH /usr/local/lib/cmake)
set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/glfw3)

include_directories(/usr/local/include glad)
include_directories(${GLFW_INCLUDE_DIRS})


set(SOURCE_FILES main.cpp game.cpp game.h shader.cpp shader.h texture.cpp texture.h resource_manager.cpp resource_manager.h stb_image.cpp stb_image.h glad.c sprite_renderer.cpp sprite_renderer.h game_object.cpp game_object.h game_level.cpp game_level.h particle_generator.cpp particle_generator.h post_processor.cpp post_processor.h fbox_object.cpp fbox_object.h food_object.cpp food_object.h cat_object.cpp cat_object.h button_object.cpp button_object.h)
add_executable(CatFoodIdle ${SOURCE_FILES})

target_link_libraries(CatFoodIdle -lglfw3 -lglu32 -lOpenGL32 -lfreeGLUT -lglut)
