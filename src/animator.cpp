#include <ass2/animator.hpp>

#include <iostream>

#include <cmath>
#include <stack>

// Repeat each animation every 10 seconds
const float PERIOD = 10.f;
float t = 0.0f;
float c = 0.0f;
bool swap = false;
bool rotation = true;

// Create a translation vector based on a parameter 't' and the control points of a Bézier curve
glm::vec3 cubic(const std::vector<glm::vec3> &p, float t) {
    float s = 1 - t;
    auto pos = (s * s *s * p[0]) + (t * s * s * p[1]) + (t * t * s * p[2]) + (t * t * t * p[3]);
    return pos;
}


void animateCircle (std::vector<glm::mat4> &b, float now, const renderer_t renderer) {
    if (c >= 0.99f) {
        rotation = rotation ? false : true;
    }
    if (rotation) {
        c += 0.01f;
    } else {
        c = 0.0f;
        rotation = true;
    }

    std::vector<glm::vec3> s = {
        {0, 70, 1.5,},
        {-1000, 70, -205,},
        {1000, 70, -209.6,},
        {0, 70, 1.5}
    };
    glm::vec3 translation = cubic(s , c);

    std::string name = "pointLights[" + std::to_string((int )now ); 
    name = name + "].position";

    glUniform3f(glGetUniformLocation(renderer.program, name.c_str()), translation.x,
    translation.y, translation.z);


    b[now] = glm::translate(glm::mat4(1), translation);
    
}

// Make our animator_t struct (mainly important for using the spline)
animator_t make_animator() {
    int scale = 35;

    bspline_t lemniscate = {
            std::vector<glm::vec3> {
                    {0, 0, 0,},
                    {2, 0, 2,},
                    {-2, -0, 2,},
                    {0, 0, 0}
            },

            std::vector<glm::vec3> {
                    {0, 0, 0,},
                    {-20, 0, -3,},
                    {0, -0, 0,},
                    {0, 0, 0}
            },
    };

    for (glm::vec3 &p : lemniscate.s1) {
        p *= scale;
    }

    for (glm::vec3 &p : lemniscate.s2) {
        p *= scale;
    }

    return animator_t{ lemniscate, lemnisacte_t{ }};
}

// Do our animation.
void animate(const animator_t &animator, scene_t &scene, float now, const renderer_t &renderer) {
    std::stack<node_t *> animatables{{ &scene.root }};
    while (!animatables.empty()) {
        node_t *n = animatables.top();
        animatables.pop();
        switch (n->kind) {
            case node_t::EVA:
                //n->transform = animate(animator.spline_animation, now);
                break;
            case node_t::ANGEL:
                n->transform = glm::translate(glm::mat4(1), {0.0f, 50.0f, 3.0f}) * glm::scale(glm::mat4(1), {0.3f, 0.3f, 0.3f});
                n->transform = glm::translate(n->transform, {0.0f, sin(now) * 20  + 30, 0.0f});
                n->rotation.y = 0;
                for (int i = 0; i < 30; ++i) {
                    animateCircle(n->cubes.cubePositions, (float) i, renderer);
                    n->cubes.cubePositions[i] = glm::translate(n->cubes.cubePositions[i], {0.0f, sin(glfwGetTime()) * 10 + 10, 0.0f});
                }
                break;
            case node_t ::BARRIER:
                n->transform = glm::translate(glm::mat4(1), {0.1f, 0.0f, -200});
            default:
                break;
        }

        for (node_t &c: n->children) {
            animatables.push(&c);
        }
    }
}
