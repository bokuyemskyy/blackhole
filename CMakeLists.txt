cmake_minimum_required(VERSION 3.31)

project(BlackHole)

set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/third_party")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/src")
set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/shader")

set(GLAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad")

add_executable(${PROJECT_NAME} 
    src/main.cpp
    "${GLAD_DIR}/src/glad.c")

target_include_directories(${PROJECT_NAME} PRIVATE 
    "${GLAD_DIR}/include"
)
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
    COMPILE_OPTIONS -Werror
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

find_package(OpenGL REQUIRED)

include(FetchContent)
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        master 
)
FetchContent_MakeAvailable(glm)

target_link_libraries(${PROJECT_NAME}
glfw
glm::glm
OpenGL::GL
)

