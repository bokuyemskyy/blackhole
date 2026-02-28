#pragma once

#include <glad/glad.h>
#include <string>
#include <iostream>
#include "stb_image.h"

class AccretionDisk {
public:
    GLuint texture{};

    float rotation_speed; // rad/M
    float inner_radius;
    float outer_radius;

    AccretionDisk(const std::string &texture_path, float rotation_speed_,
                  float inner_radius_, float outer_radius_) : rotation_speed(rotation_speed_),
                                                              inner_radius(inner_radius_), outer_radius(outer_radius_) {
        int width, height, channels;

        unsigned char *data = stbi_load(texture_path.c_str(), &width, &height, &channels, 4);

        if (!data) {
            std::cerr << "Failed to load accretion disk texture: "
                    << texture_path << "\n";
            return;
        }

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);


        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        GLfloat maxAnisotropy = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
    }

    AccretionDisk(const AccretionDisk &) = delete;

    AccretionDisk &operator=(const AccretionDisk &) = delete;

    AccretionDisk(AccretionDisk &&other) noexcept
        : texture(other.texture),
          rotation_speed(other.rotation_speed),
          inner_radius(other.inner_radius),
          outer_radius(other.outer_radius) {
        other.texture = 0;
    }

    AccretionDisk &operator=(AccretionDisk &&other) noexcept {
        if (this != &other) {
            glDeleteTextures(1, &texture);

            texture = other.texture;
            rotation_speed = other.rotation_speed;
            inner_radius = other.inner_radius;
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
