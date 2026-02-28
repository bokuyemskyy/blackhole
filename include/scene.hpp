#pragma once

#include "accretion_disk.hpp"
#include "skybox.hpp"
#include "black_hole.hpp"
#include "camera.hpp"

class Scene {
public:
    Scene(Camera camera, Skybox skybox, BlackHole black_hole,
          AccretionDisk accretion_disk) : m_camera(camera), m_skybox(std::move(skybox)),
                                          m_black_hole(black_hole), m_accretion_disk(std::move(accretion_disk)) {
    }

    [[nodiscard]] Skybox &getSkybox() { return m_skybox; }
    [[nodiscard]] Camera &getCamera() { return m_camera; }
    [[nodiscard]] BlackHole &getBlackHole() { return m_black_hole; }
    [[nodiscard]] AccretionDisk &getAccretionDisk() { return m_accretion_disk; }

private:
    Camera m_camera;
    Skybox m_skybox;
    BlackHole m_black_hole;
    AccretionDisk m_accretion_disk;
};
