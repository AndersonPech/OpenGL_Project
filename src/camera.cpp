#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <ass2/camera.hpp>
#include <ass2/scene.hpp>

namespace {

    const float CAMERA_SPEED = 10.f;
    const float INVERT_X = -1.f;
    const float INVERT_Y = 1.f;
    bool fps = false;
    bool animation = false;
    float c = 0.0f;
    bool rotation = true;


    // Create a translation vector based on a parameter 't' and the control points of a Bézier curve
    glm::vec3 cubic(const std::vector<glm::vec3> &p, float t) {
        float s = 1 - t;
        auto pos = (s * s *s * p[0]) + (t * s * s * p[1]) + (t * t * s * p[2]) + (t * t * t * p[3]);
        return pos;
    }


    // Animate our figure-8 movement using a bspline.
    glm::vec3 animate() {
        
        if (c >= 0.99f) {
            rotation = rotation ? false : true;
        }
        if (rotation) {
            c += 0.001f;
        } else {
            c = 0.0f;
            rotation = true;
        }
        std::vector<glm::vec3> s = {
            {0, 300, -100.5,},
            {-600, 300, -10,},
            {600, 300, -10.6,},
            {0, 300, 105}
        };
        return cubic(s , c);
        
    }

    glm::vec3 sidev(float p, float y) {
        auto cy = std::cos(y);
        auto sy = std::sin(y);

        return glm::normalize(glm::vec3{ cy, 0, -sy });
    }

    glm::vec3 upv(float p, float y) {
        auto cp = std::cos(p);
        auto cy = std::cos(y);
        auto sp = std::sin(p);
        auto sy = std::sin(y);

        return glm::normalize(glm::vec3{ -sp * sy, cp, -cy * sp });
    }

    glm::vec3 frontv(float p, float y) {
        auto cp = std::cos(p);
        auto cy = std::cos(y);
        auto sp = std::sin(p);
        auto sy = std::sin(y);

        return glm::normalize(glm::vec3{ cp * sy, sp, cy * cp });
    }

    void update_cam_angles(camera_t &cam, GLFWwindow *win, float dt) {
        glm::dvec2 ms_pos;
        glfwGetCursorPos(win, &ms_pos[0], &ms_pos[1]);
        static glm::vec2 prev_pos = ms_pos;

        glm::vec2 delta = glm::radians(glm::vec2(ms_pos) - prev_pos);

        prev_pos = ms_pos;

        if (fps) cam.pos.y = 3;

        if (animation) {
            cam.pos = animate();
            cam.yaw = 0;
            cam.pitch = -5;
        } else {
           cam.yaw += INVERT_X * 1/CAMERA_SPEED * delta.x;
           cam.pitch += INVERT_Y * 1/CAMERA_SPEED * delta.y; 
        }

        
    }

    void update_cam_pos(camera_t &cam, GLFWwindow *win, float dt) {
        auto front = -frontv(cam.pitch, cam.yaw);
        auto right = sidev(cam.pitch, cam.yaw);
        float step = dt * CAMERA_SPEED;

        if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
            cam.pos += front * step;
        }
        if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
            cam.pos -= front * step;
        }
        if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
            cam.pos += right * step;
        }
        if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
            cam.pos -= right * step;
        }

        if (glfwGetKey(win, GLFW_KEY_F) == GLFW_PRESS) {
            fps = true;
            if (fps) cam.pos.y = 30;
        }

        if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) {
            fps = false;
            animation = false;
        }
        if (glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS) {
            animation = true;
            if (animation) {
                cam = make_camera({ 0, 20, -100 }, { 0, 0, 0 });
            }
        }
    }
}

camera_t make_camera(glm::vec3 pos, glm::vec3 target) {
    glm::vec3 front = glm::normalize(target - pos);

    // calculate the yaw and pitch from the front vector
    // pretty sure there's a bug here
    float yaw = glm::degrees(std::atan2(front.x, -front.z));
    float pitch = glm::degrees(std::asin(front.y));

    return { pos, 0, 0 };
}

void delete_camera(camera_t &c) {
    // nothing to do as this is a trivially destructible type
    (void)c;
}


void update_camera(camera_t &cam, GLFWwindow *window, float dt) {
    update_cam_angles(cam, window, dt);
    update_cam_pos(cam, window, dt);
}

glm::mat4 get_view(const camera_t &cam) {
    auto s = sidev(cam.pitch, cam.yaw);
    auto u = upv(cam.pitch, cam.yaw);
    auto f = frontv(cam.pitch, cam.yaw);
    auto o = glm::vec3(-glm::dot(cam.pos, s), -glm::dot(cam.pos, u), -glm::dot(cam.pos, f));
    auto view = glm::mat4{
            s.x, u.x, f.x, 0,
            s.y, u.y, f.y, 0,
            s.z, u.z, f.z, 0,
            o.x, o.y, o.z, 1
    };

    return view;
}