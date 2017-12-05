cmake_minimum_required(VERSION 2.8)

project(Aqua)

find_package(OpenGL REQUIRED)
find_package(SDL 1.2 REQUIRED)
find_package(SDL_image 1.2 REQUIRED)
find_package(Boost 1.36 REQUIRED filesystem)

file(GLOB AquaSrc "src/*.h" "src/*.cpp")
add_executable(${PROJECT_NAME} ${AquaSrc})

add_definitions(-DDATADIR="data" -DGL_GLEXT_PROTOTYPES)

include_directories(${OPENGL_INCLUDE_DIR}
                    ${SDL_INCLUDE_DIR}
                    ${SDL_IMAGE_INCLUDE_DIR}
                    ${Boost_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES}
                                      ${SDL_LIBRARY}
                                      ${SDL_IMAGE_LIBRARY}
                                      ${Boost_LIBRARIES})
