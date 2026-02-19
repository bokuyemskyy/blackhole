#include "renderer.hpp"
#include <glm/ext/matrix_float4x4.hpp>

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
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_shader.use();

    m_scene.getSkybox().bind(GL_TEXTURE0);

    glUniform1i(glGetUniformLocation(m_shader.ID, "uSkybox"), 0);

    auto identity = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(m_shader.ID, "uCameraInverse"),
                       1, GL_FALSE, &identity[0][0]);

    glUniform2f(glGetUniformLocation(m_shader.ID, "uResolution"), (float) width, (float) height);
    glUniform1f(glGetUniformLocation(m_shader.ID, "uTime"), (float) glfwGetTime());

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwSwapBuffers(window);
}
