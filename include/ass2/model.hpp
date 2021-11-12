#ifndef COMP3421_ASS2_MODEL_HPP
#define COMP3421_ASS2_MODEL_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <vector>
#include <string>

const GLenum index_type = GL_UNSIGNED_INT;
/**
 * A struct that represents the raw geometry of a model.
 * Does not contain all the usual graphics things, yet.
 */
struct mesh_t {
    GLuint vao;
    GLuint vbo;
    GLuint ibo;
    GLuint nverts;
    GLuint material_id;
    GLuint specular_map;
};

// struct cubeAnimation {
//     glm::mat4 position;
//     float point;
// }

struct cube {
    GLuint vao;
    GLuint vbo;
    GLuint ibo;
    int nverts;
    std::vector<glm::mat4> cubePositions;
    glm::mat4 location;
};



/**
 * A struct that represents what a mesh is "made out of"
 * This can be colours, textures, and even more...
 */
struct material_t {
    GLuint texture;
    glm::vec4 color;
};

/**
 * A representation of a model loaded from an artist's tool
 */
struct model_t {
    std::vector<mesh_t> meshes;
    std::vector<material_t> materials;
};


struct material_param_t {
    std::string diff_map_path;
};

struct volume_param_t {
    float width = 1;
    float height = 1;
    float depth = 1;
};

/**
 * Load a model from the .obj format
 * TODO complete this
 *
 * @param path The path to the model's .obj file
 * @return A freshly initialised model, with everything uploaded to the GPU already
 */
model_t load_obj(const std::string &path);

/**
 * Destroy a model, freeing all of its GPU resources
 * @param m The model to destroy
 */
void destroy_obj(model_t &m);

model_t make_volume_mesh(float a, float b, float c, const std::string &p);

#endif //COMP3421_TUTORIAL_05_MODEL_HPP
