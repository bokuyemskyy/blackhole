#include "renderer.hpp"

void Renderer::setupMesh() {
    float quadVertices[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f, 1.0f,

        -1.0f, 1.0f,
        1.0f, -1.0f,
        1.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
}

void Renderer::render(GLFWwindow *window) {
    calcluateDeltaTime();
    updateFps(window);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    float time = (float) glfwGetTime();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader.use();

    float u_black_hole_mass = m_scene.getBlackHole().mass;
    m_shader.setFloat("uBlackHoleMass", u_black_hole_mass);

    glm::mat4 u_camera_inv_view_proj = m_scene.getCamera().getInvViewProj(aspect);
    m_shader.setMat4("uCameraInvViewProj", u_camera_inv_view_proj);
    glm::vec3 u_camera_position = m_scene.getCamera().getPosition();
    m_shader.setVec3("uCameraPosition", u_camera_position);

    m_scene.getSkybox().bind(GL_TEXTURE0);
    m_shader.setInt("uSkyboxTexture", 0);
    float u_skybox_escape_radius = m_scene.getSkybox().escape_radius;
    m_shader.setFloat("uSkyboxEscapeRadius", u_skybox_escape_radius);

    m_scene.getAccretionDisk().bind(GL_TEXTURE1);
    m_shader.setInt("uDiskTexture", 1);
    float u_disk_inner_radius = m_scene.getAccretionDisk().inner_radius;
    m_shader.setFloat("uDiskInnerRadius", u_disk_inner_radius);
    float u_disk_outer_radius = m_scene.getAccretionDisk().outer_radius;
    m_shader.setFloat("uDiskOuterRadius", u_disk_outer_radius);
    float u_disk_rotation_speed = m_scene.getAccretionDisk().rotation_speed;
    m_shader.setFloat("uDiskRotationSpeed", u_disk_rotation_speed);

    glm::vec2 u_resolution = glm::vec2(width, height);
    m_shader.setVec2("uResolution", u_resolution);

    m_shader.setFloat("uTime", time);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwSwapBuffers(window);
}

void Renderer::calcluateDeltaTime() {
    double current_frame_time = glfwGetTime();
    m_delta_time = current_frame_time - m_last_frame_time;
    m_last_frame_time = current_frame_time;
}

void Renderer::updateFps(GLFWwindow *window) {
    m_frametime_sum -= m_frametimes[m_frame_index];
    m_frametimes[m_frame_index] = m_delta_time;
    m_frametime_sum += m_delta_time;
    m_frame_index = (m_frame_index + 1) % FPS_SAMPLE_COUNT;

    if (m_frames_accumulated < FPS_SAMPLE_COUNT)
        m_frames_accumulated++;

    double avgFrameTime = m_frametime_sum / m_frames_accumulated;
    double fps = 1.0 / avgFrameTime;

    std::string title = "Black Hole - FPS: " + std::to_string(static_cast<int>(fps));
    glfwSetWindowTitle(window, title.c_str());
}

void Renderer::processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    m_scene.getCamera().processInput(window, m_delta_time);
}
