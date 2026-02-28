#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "shader.hpp"
#include "scene.hpp"
#include "skybox.hpp"
#include <renderer.hpp>

const int W_WIDTH = 1400;
const int W_HEIGHT = 1000;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);

  Renderer *renderer = (Renderer *) glfwGetWindowUserPointer(window);

  if (renderer) {
    renderer->render(window);
  }
}

int main() {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Black Hole", NULL, NULL);

  if (!window) {
    std::cout << "Failed to create window\n";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGL()) {
    std::cout << "Failed to initialize GLAD\n";
    return -1;
  }

  int fbWidth, fbHeight;
  glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
  glViewport(0, 0, fbWidth, fbHeight);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  std::vector<std::string> skybox_faces = {
    "textures/skybox/px.png",
    "textures/skybox/nx.png",
    "textures/skybox/py.png",
    "textures/skybox/ny.png",
    "textures/skybox/pz.png",
    "textures/skybox/nz.png"
  };
  std::string accretion_disk_texture = "textures/disk.png";

  Skybox skybox{skybox_faces, 300.0f};
  Camera camera{{70.0f, 30.0f, 70.0f}, {-15.0f, -135.0f, 0.0f}, 10.0f};
  BlackHole black_hole{1};
  AccretionDisk accretion_disk{accretion_disk_texture, 0.3, 8, 50};

  Scene scene{camera, std::move(skybox), black_hole, std::move(accretion_disk)};

  Shader shader("shaders/fullscreen.vert", "shaders/raycast.frag");

  Renderer renderer(shader, scene);

  glfwSetWindowUserPointer(window, &renderer);

  while (!glfwWindowShouldClose(window)) {
    renderer.processInput(window);
    renderer.render(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
