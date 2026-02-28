#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <array>
#include "scene.hpp"
#include "shader.hpp"

class Renderer {
public:
    Renderer(Shader &shader, Scene &scene) : m_shader(shader), m_scene(scene) {
        setupMesh();
    }

    void render(GLFWwindow *window);

    void calcluateDeltaTime();

    void updateFps(GLFWwindow *window);

    void processInput(GLFWwindow *window);

    void setupMesh();

private:
    static constexpr int FPS_SAMPLE_COUNT = 50;
    std::array<double, FPS_SAMPLE_COUNT> m_frametimes{};
    int m_frame_index = 0;
    int m_frames_accumulated = 0;
    double m_frametime_sum;

    double m_delta_time;
    double m_last_frame_time;
    Shader &m_shader;
    Scene &m_scene;
    unsigned int VAO{}, VBO{};
};
