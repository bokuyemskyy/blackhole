#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "scene.hpp"
#include "shader.hpp"

class Renderer {
public:
    Renderer(Shader &shader, Scene &scene) : m_shader(shader), m_scene(scene) {
        setupMesh();
    }

    void render(GLFWwindow *window);

    void setupMesh();

private:
    Shader &m_shader;
    Scene &m_scene;
    unsigned int VAO{}, VBO{};
};
