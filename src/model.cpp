#include <ass2/model.hpp>

#include <tiny_obj_loader.h>

#include <chicken3421/chicken3421.hpp>

#include <iostream>

struct vertex_t {
    glm::vec3 pos;

    glm::vec3 nrm;

    glm::vec2 tc;
};

using index_t = GLuint;


// helper functions
mesh_t make_mesh(
        const std::vector<glm::vec4> &positions,
        const std::vector<glm::vec2> &texcoords,
        const std::vector<glm::vec3> &normals,
        int material_id
) {
    GLuint vao = chicken3421::make_vao();
    glBindVertexArray(vao);

    GLuint vbo = chicken3421::make_buffer();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    GLsizeiptr pos_sz = positions.size() * sizeof(glm::vec4);
    GLsizeiptr tc_sz = texcoords.size() * sizeof(glm::vec2);
    GLsizeiptr nrm_sz = texcoords.size() * sizeof(glm::vec3);

    glBufferData(
            GL_ARRAY_BUFFER,
            pos_sz + tc_sz + nrm_sz,
            nullptr,
            GL_STATIC_DRAW
    );

    glBufferSubData(GL_ARRAY_BUFFER, 0, pos_sz, positions.data());
    glBufferSubData(GL_ARRAY_BUFFER, pos_sz, tc_sz, texcoords.data());
    glBufferSubData(GL_ARRAY_BUFFER, pos_sz + nrm_sz, tc_sz, normals.data());

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void *) nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *) pos_sz);;
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *) (pos_sz + nrm_sz));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return mesh_t{ vao, vbo, (GLuint) -1, (GLuint)positions.size(), (GLuint) material_id, (GLuint) -1 };
}

GLuint load_texture(const std::string &path) {
    chicken3421::image_t teximg = chicken3421::load_image(path);
    GLint fmt = teximg.n_channels == 3 ? GL_RGB : GL_RGBA;

    GLuint tex = chicken3421::make_texture();
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, fmt, teximg.width, teximg.height, 0, fmt, GL_UNSIGNED_BYTE, teximg.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    chicken3421::delete_image(teximg);

    return tex;
}

mesh_t upload_mesh(const std::vector<vertex_t> &vertices, const std::vector<index_t> &indices, const std::string &p) {
    GLuint vao = chicken3421::make_vao();
    glBindVertexArray(vao);

    GLuint vbo = chicken3421::make_buffer();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    GLuint ibo = chicken3421::make_buffer();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_t) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void *) offsetof(vertex_t, pos));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void *) offsetof(vertex_t, tc));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,  sizeof(vertex_t), (void *) offsetof(vertex_t, nrm));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return mesh_t{ vao, vbo, ibo, (GLuint) indices.size(), load_texture(p), load_texture("res/textures/pat.jpg") };
}

model_t make_volume_mesh(float a, float b, float c, const std::string &p) {
    float w = a;
    float h = b;
    float d = c;

    float hw = w/2;
    float hh = h/2;
    float hd = d/2;

    std::vector<vertex_t> verts = {
            // assuming looking down the negative-z axis...

            // front face
            {{ -w, -h, d },  { 0,  0,  1 },  { 0,  0 }},
            {{ w,  -h, d },  { 0,  0,  1 },  { hw, 0 }},
            {{ w,  h,  d },  { 0,  0,  1 },  { hw, hh }},
            {{ -w, h,  d },  { 0,  0,  1 },  { 0,  hh }},

            // right face
            {{ w,  -h, d },  { 1,  0,  0 },  { 0,  0 }},
            {{ w,  -h, -d }, { 1,  0,  0 },  { hd, 0 }},
            {{ w,  h,  -d }, { 1,  0,  0 },  { hd, hh }},
            {{ w,  h,  d },  { 1,  0,  0 },  { 0,  hh }},


            // back face
            {{ w,  -h, -d }, {0, 0, -1}, { 0,  0 }},
            {{ -w, -h, -d }, {0, 0, -1}, { hw, 0 }},
            {{ -w, h,  -d }, {0, 0, -1}, { hw, hh }},
            {{ w,  h,  -d }, {0, 0, -1}, { 0,  hh }},

            // left face
            {{ -w, -h, -d }, {-1, 0, 0}, { 0,  0 }},
            {{ -w, -h, d }, {-1, 0, 0}, { hd, 0 }},
            {{ -w, h,  d }, {-1, 0, 0}, { hd, hh }},
            {{ -w, h,  -d }, {-1, 0, 0}, { 0,  hh }},

            // top face
            {{ -w, h,  d }, {0, 1, 0}, { 0,  0 }},
            {{ w,  h,  d }, {0, 1, 0}, { hw, 0 }},
            {{ w,  h,  -d }, {0, 1, 0}, { hw, hd }},
            {{ -w, h,  -d }, {0, 1, 0}, { 0,  hd }},

            // bottom face
            {{ -w, -h, -d }, {0, -1, 0}, { 0,  0 }},
            {{ w,  -h, -d }, {0, -1, 0}, { hw, 0 }},
            {{ w,  -h, d }, {0, -1, 0}, { hw, hd }},
            {{ -w, -h, d }, {0, -1, 0}, { 0,  hd }},
    };
    std::vector<index_t> indices = {
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
    model_t model;
    model.meshes.emplace_back(upload_mesh(verts, indices, p));
    return model;
}

// 'new' functions:
model_t load_obj(const std::string &path) {

    // load object data using tinyobjloader
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string str_warn, str_err;
    std::string mtl_base = path.substr(0, path.find_last_of('/') + 1);

    bool did_load = tinyobj::LoadObj(&attrib, &shapes, &materials, &str_warn, &str_err, path.c_str(), mtl_base.c_str());
    chicken3421::expect(did_load && str_warn.empty() && str_err.empty(), str_warn + str_err);

    // load the meshes
    std::vector<mesh_t> meshes;
    for (const tinyobj::shape_t &shape : shapes) {
        std::vector<glm::vec4> positions;
        std::vector<glm::vec2> texcoords;
        std::vector<glm::vec3> normals;

        for (const tinyobj::index_t &index : shape.mesh.indices) {
            // an example of how vertices are laid out (roughly)
            positions.emplace_back(
                    attrib.vertices[3 * index.vertex_index],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2],
                    1
            );

            texcoords.emplace_back(
                    attrib.texcoords[2 * index.texcoord_index],
                    attrib.texcoords[2 * index.texcoord_index+ 1]
            );

            normals.emplace_back(
                    attrib.normals[3 * index.normal_index],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
            );
        }

        // only getting the first material - won't work in all cases
        int material = shape.mesh.material_ids.empty() ? -1 : shape.mesh.material_ids[0];

        meshes.emplace_back(make_mesh(positions, texcoords, normals, material));
    }

    // load the materials
    std::vector<material_t> mats;
    for (const tinyobj::material_t &mat: materials) {
        // for now think of diffuse textures as just the 'standard' colour.
        GLuint tex = mat.diffuse_texname.empty() ? 0 : load_texture(mtl_base + mat.diffuse_texname);
        glm::vec4 colour = { mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], 1 };

        mats.emplace_back(material_t{ tex, colour });
    }

    return {meshes, mats};
}

void destroy_obj(model_t &m) {
    for (mesh_t &mesh: m.meshes) {
        chicken3421::delete_vao(mesh.vao);
        chicken3421::delete_buffer(mesh.vbo);

        mesh.vbo = mesh.vao = 0;
        mesh.nverts = 0;
    }

    for (material_t &mat: m.materials) {
        chicken3421::delete_texture(mat.texture);
        mat.texture = 0;
    }
}

