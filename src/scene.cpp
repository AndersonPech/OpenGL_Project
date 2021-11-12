#include <ass2/scene.hpp>
#include <vector>

#include <chicken3421/chicken3421.hpp>
#include <ass2/model.hpp>
#include <ass2/scene.hpp>


const char *B = "res/objs/eva/Unit01.obj";
const char *A = "res/objs/ramiel/ramiel.obj";
const char *C = "res/textures/crate.png";
const char *D = "res/textures/metal.png";
using namespace std;

cube createCube();

glm::mat4 calculate_angel() {
    return glm::translate(glm::mat4(1), {0.0f, 50.0f, 1.0f});
}
scene_t make_scene() {
    camera_t cam = make_camera({ 0, 1, -130 }, { 0, 0, 0 });
    node_t barrier {
            node_t::BARRIER,
            make_volume_mesh(10, 10, 10, D),
            glm::translate(glm::mat4(1), glm::vec3(0, 100, 1)),
            {},
            {},
            {}
        };
        
    node_t gt {
        node_t::EVA,
        load_obj(B),
        glm::translate(glm::mat4(1), {0.1f, 0.0f, -30}),
        {},
        {},
        {}
    };

    node_t angel {
        node_t::ANGEL,
        load_obj(A),
        calculate_angel(),
        {},
        {createCube()},
        {0.0f, 50.0f, 50.0f}
    };

    node_t track = {
        node_t::TRACK,
        make_volume_mesh(500, 0, 500, C),
        glm::mat4(1),
        {gt, angel, barrier},
        {},
        {}
    };

    directional_light_t sun = {
        glm::vec3(0, 30, -100),
        glm::vec3(0.2, 0.2, 0.2 ),
        glm::vec3(0.9, 0.9, 0.9)
    };


    return scene_t{ cam, sun, track };
}

/**
 * Create the surrounding thing
 * 
 */ 
cube createCube() {
    float vertices[] = {
        -0.5f, -0.5f, 0.5f,   0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.5f,   1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, 0.5f,   1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
         -0.5f,  0.5f, 0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
  
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  -0.5f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
    
        0.5f, -0.5f,  -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f,   0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f,    0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
 
        -0.5f,  -0.5f,  -0.5f, 1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f,  -0.5f, 0.5f,  1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,  0.0f, 1.0f,     -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f,  0.0f, 1.0f,    -1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f,  0.0f, 1.0f,     0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f,  1.0f, 1.0f,      0.0f, 1.0f, 0.0f,
        0.5f, 0.5f,  -0.5f,  1.0f, 0.0f,    0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f,  -0.5f,  1.0f, 0.0f,   0.0f, 1.0f, 0.0f,

        -0.5f,  -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f, 0.0f,
        0.5f,  -0.5f, -0.5f,  1.0f, 1.0f,   0.0f, -1.0f, 0.0f,
        0.5f,  -0.5f,  0.5f,  1.0f, 0.0f,   0.0f, -1.0f, 0.0f,
        -0.5f,  -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
   
    };
    // world space positions of our cubes
    vector<glm::mat4> cubePositions = {};

    for (int i = 0;  i < 30; ++i) {
        cubePositions.push_back(glm::mat4(1));
    }



    std::vector<GLuint> indices = {
                0, 1, 2,
                0, 2, 3,

                4, 5, 6,
                4, 6, 7,

                8, 9, 10,
                8, 10, 11,

                12, 13, 14,
                12, 14, 15,

                16, 17, 18,
                16, 18, 19,

                20, 21, 22,
                20, 22, 23
        };

    GLuint vao = chicken3421::make_vao();
    glBindVertexArray(vao);

    GLuint vbo = chicken3421::make_buffer();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

    GLuint ibo = chicken3421::make_buffer();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return {vao, vbo, ibo, indices.size(), cubePositions, glm::translate(glm::mat4(1), { 0, 1.1f, 0 }),};
}