cmake_minimum_required(VERSION 3.10)
project(raytracer)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(SANITIZER_FLAGS "-fsanitize=address,undefined -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SANITIZER_FLAGS}")

add_executable(main src/main.c src/image.c src/colour.c src/vector.c src/scene.c src/raytracer.c src/bvh.c)

target_link_libraries(main PRIVATE m)
