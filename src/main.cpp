#include <thread>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <chicken3421/chicken3421.hpp>

#include <ass2/animator.hpp>
#include <ass2/camera.hpp>
#include <ass2/memes.hpp>
#include <ass2/renderer.hpp>
#include <ass2/scene.hpp>

/**
 * Returns the difference in time between when this function was previously called and this call.
 * @return A float representing the difference between function calls in seconds.
 */
float time_delta();

/**
 * Returns the current time in seconds.
 * @return Returns the current time in seconds.
 */
float time_now();

void update_sunlight(glm::vec3 &sun, GLFWwindow *win, float dt) {
    static glm::vec2 EXTENT = {170, 10};
    static float INCREMENT = 25;
    static float curr_angle = 90;

    float change = 0;
    if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS) {
        change = curr_angle >= EXTENT.x ? 0 : INCREMENT * dt;
    } else if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) {
        change = curr_angle <= EXTENT.y ? 0 : -INCREMENT * dt;
    }

    curr_angle += change;

    sun = glm::rotate(glm::mat4(1), glm::radians(change), {0, 0, 1}) * glm::vec4(sun, 1);
}


int main() {
    GLFWwindow *win = marcify(chicken3421::make_opengl_window(1280, 720, "Tutorial 05"));

    // TODO turn this on or off?
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    scene_t scene = make_scene();
    animator_t animator = make_animator();
    renderer_t renderer = make_renderer(glm::perspective(glm::pi<double>() / 3, 1280.0 / 720, 0.1, 1000.0));
    renderer_cube rendererCube = makeCubeProgram(glm::perspective(glm::pi<double>() / 3, 1280.0 / 720, 0.1, 1000.0));


    while (!glfwWindowShouldClose(win)) {
        float dt = time_delta();

        update_camera(scene.cam, win, dt);
        update_sunlight(scene.sunlight.dir, win, dt);
        animate(animator, scene, time_now(), renderer);
        render(renderer, scene, rendererCube);
        

        glfwSwapBuffers(win);
        glfwPollEvents();

        // not entirely correct as a frame limiter, but close enough
        // it would be more correct if we knew how much time this frame took to render
        // and calculated the distance to the next "ideal" time to render and only slept that long
        // the current way just always sleeps for 16.67ms, so in theory we'd drop frames
        std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(1000.f / 40));
    }

    // deleting the whole window also removes the opengl context, freeing all our memory in one fell swoop.
    chicken3421::delete_opengl_window(win);

    return EXIT_SUCCESS;
}


float time_delta() {
    static float then = time_now();
    float now = time_now();
    float dt = now - then;
    then = now;
    return dt;
}

float time_now() {
    return (float)glfwGetTime();
}