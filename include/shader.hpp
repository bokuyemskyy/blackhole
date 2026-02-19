#pragma once

#include <string>
#include <glm/ext/matrix_float4x4.hpp>

class Shader {
public:
    unsigned int ID;

    Shader(const char *vertexShaderPath, const char *fragmentShaderPath);

    void use();

    void setInt(const std::string &name, int value);

    void setFloat(const std::string &name, float value);

    void setVec2(const std::string &name, float x, float y);

    void setMat4(const std::string &name, const glm::mat4 &mat);
};
