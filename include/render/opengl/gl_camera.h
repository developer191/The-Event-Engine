#pragma once

#include "util/Definitions.h"
#include "render/Camera.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>

#include "SDL.h"

class GlCamera : public Camera {
protected:
    uint32 velocity;

    glm::vec3 up_vector;
    glm::vec3 position;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 vp_matrix;

    std::map<int, bool> tracked_keys;

public:
    bool initialize();
    void onInput(std::shared_ptr<void>);
    void update(std::shared_ptr<void>);
    // void set_position();
    glm::mat4 get_vp_matrix();
};