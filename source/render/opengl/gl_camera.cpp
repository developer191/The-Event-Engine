#include "render/opengl/gl_camera.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <utility>

#include "event_system/Dispatcher.h"
#include "event_system/Subscriber.h"

#include "util/Definitions.h"

bool GlCamera::initialize() {
    velocity = 1;
    up_vector = glm::vec3(0.0, 1.0, 0.0);
    position = glm::vec3(4, 3, -3);

    Subscriber *input_subscriber = new Subscriber(this);
    input_subscriber->method = std::bind(&GlCamera::onInput, this, std::placeholders::_1);
    Dispatcher::GetInstance()->AddEventSubscriber(input_subscriber, "EVENT_INPUT");

    Subscriber *update_subscriber = new Subscriber(this);
    update_subscriber->method = std::bind(&GlCamera::update, this, std::placeholders::_1);
    Dispatcher::GetInstance()->AddEventSubscriber(update_subscriber, "EVENT_COMPONENT_UPDATE");

    tracked_keys[SDLK_UP] = false;
    tracked_keys[SDLK_DOWN] = false;
    tracked_keys[SDLK_LEFT] = false;
    tracked_keys[SDLK_RIGHT] = false;

    return true;
}

void GlCamera::onInput(std::shared_ptr<void> event) {
    std::pair<int, bool> *pair = (std::pair<int, bool> *)event.get();
    tracked_keys[pair->first] = pair->second;
}

void GlCamera::update(std::shared_ptr<void> event_data) {
    float32 delta_time = *(float *)event_data.get();

    float32 z_movement = 0;
    float32 x_movement = 0;

    if (tracked_keys[SDLK_UP]) {
        z_movement += velocity * delta_time;
    }
    if (tracked_keys[SDLK_DOWN]) {
        z_movement -= velocity * delta_time;
    }
    if (tracked_keys[SDLK_LEFT]) {
        x_movement += velocity * delta_time;
    }
    if (tracked_keys[SDLK_RIGHT]) {
        x_movement -= velocity * delta_time;
    }

    position.x += x_movement;
    position.z += z_movement;

    // std::cout << position_.x << " " << position_.z << std::endl;

    projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
    glm::mat4 view = glm::lookAt(position, glm::vec3(0.0, 0.0, 0.0), up_vector);
    vp_matrix = projection * view;
}

glm::mat4 GlCamera::get_vp_matrix() { return vp_matrix; }