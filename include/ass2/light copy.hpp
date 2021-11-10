#ifndef COMP3421_TUTORIAL_07_LIGHT_HPP
#define COMP3421_TUTORIAL_07_LIGHT_HPP

#include <glm/glm.hpp>

struct directional_light_t {
    // intensities of the light
    glm::vec3 dir;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    // glm::vec3 specular;
};

#endif //COMP3421_TUTORIAL_07_LIGHT_HPP
