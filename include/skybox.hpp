#pragma once

#include <glad/glad.h>
#include <vector>
#include <string>
#include <iostream>
#include "stb_image.h"

class Skybox {
public:
    GLuint cubemap_tex;
    float escape_radius;

    Skybox(const std::vector<std::string> &faces, float escape_radius_) : escape_radius(escape_radius_) {
        if (faces.size() != 6) {
            std::cerr << "Skybox requires exactly 6 textures!\n";
            return;
        }

        glGenTextures(1, &cubemap_tex);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_tex);

        int width, height, channels;
        for (unsigned int i = 0; i < 6; i++) {
            unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
            if (data) {
                GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            } else {
                std::cerr << "Failed to load skybox texture: " << faces[i] << "\n";
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    Skybox(const Skybox &) = delete;

    Skybox &operator=(const Skybox &) = delete;

    Skybox(Skybox &&other) noexcept
        : cubemap_tex(other.cubemap_tex), escape_radius(other.escape_radius) {
        other.cubemap_tex = 0;
        other.escape_radius = 0;
    }

    Skybox &operator=(Skybox &&other) noexcept {
        if (this != &other) {
            glDeleteTextures(1, &cubemap_tex);

            cubemap_tex = other.cubemap_tex;
            other.cubemap_tex = 0;
            escape_radius = other.escape_radius;
            other.escape_radius = 0;
        }
        return *this;
    }

    ~Skybox() {
        if (cubemap_tex != 0)
            glDeleteTextures(1, &cubemap_tex);
    }

    void bind(GLenum unit = GL_TEXTURE0) const {
        glActiveTexture(unit);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_tex);
    }

    void unbind() const {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
};
