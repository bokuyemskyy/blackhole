#pragma once

#include <glad/glad.h>
#include <string>
#include <iostream>
#include "stb_image.h"

class AccretionDisk {
public:
    GLuint texture{};

    float rotation_speed; // rad/M
    float diameter;
    float outer_radius;

    AccretionDisk(const std::string &texture_path, float rotation_speed_,
                  float diameter_) : rotation_speed(rotation_speed_), diameter(diameter_) {
        int width, height, channels;

        unsigned char *data = stbi_load(texture_path.c_str(), &width, &height, &channels, 0);

        if (!data) {
            std::cerr << "Failed to load accretion disk texture: "
                    << texture_path << "\n";
            return;
        }

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
    }

    AccretionDisk(const AccretionDisk &) = delete;

    AccretionDisk &operator=(const AccretionDisk &) = delete;

    AccretionDisk(AccretionDisk &&other) noexcept
        : texture(other.texture),
          rotation_speed(other.rotation_speed),
          diameter(other.diameter) {
        other.texture = 0;
    }

    AccretionDisk &operator=(AccretionDisk &&other) noexcept {
        if (this != &other) {
            glDeleteTextures(1, &texture);

            texture = other.texture;
            rotation_speed = other.rotation_speed;
            diameter = other.diameter;
            outer_radius = other.outer_radius;

            other.texture = 0;
        }
        return *this;
    }

    ~AccretionDisk() {
        if (texture != 0)
            glDeleteTextures(1, &texture);
    }

    void bind(GLenum unit = GL_TEXTURE0) const {
        glActiveTexture(unit);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    void unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};
