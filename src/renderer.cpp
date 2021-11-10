#include <ass2/renderer.hpp>

#include <stack>
#include <iostream>


#include <chicken3421/chicken3421.hpp>

const char *VERT_PATH = "res/shaders/shaders.vert";
const char *FRAG_PATH = "res/shaders/shaders.frag";
const char *CUBE_FRAG_PATH = "res/shaders/shaderscube.frag";
const char *CUBE_VERT_PATH = "res/shaders/shaderscube.vert";

renderer_t make_renderer(const glm::mat4 &projection) {
    std::vector<GLuint> program;
    GLuint vs = chicken3421::make_shader(VERT_PATH, GL_VERTEX_SHADER);
    GLuint fs = chicken3421::make_shader(FRAG_PATH, GL_FRAGMENT_SHADER);

    GLuint render_program = chicken3421::make_program(vs, fs);

    chicken3421::delete_shader(vs);
    chicken3421::delete_shader(fs);

    GLint p_loc = chicken3421::get_uniform_location(render_program, "p");
    GLint v_loc = chicken3421::get_uniform_location(render_program, "v");
    GLint m_loc = chicken3421::get_uniform_location(render_program, "m");

    GLint light_pos_loc = chicken3421::get_uniform_location(render_program, "light_pos");
    GLint amb_loc = chicken3421::get_uniform_location(render_program, "light_ambient");
    GLint diff_loc = chicken3421::get_uniform_location(render_program, "light_diffuse");
    GLuint specular = chicken3421::get_uniform_location(render_program, "u_specular");

    return renderer_t{ projection, render_program, p_loc, v_loc, m_loc, light_pos_loc, amb_loc, diff_loc, specular };
}

renderer_cube makeCubeProgram(const glm::mat4 &projection) {
    GLuint vs = chicken3421::make_shader(CUBE_VERT_PATH, GL_VERTEX_SHADER);
    GLuint fs = chicken3421::make_shader(CUBE_FRAG_PATH, GL_FRAGMENT_SHADER);

    GLuint render_program = chicken3421::make_program(vs, fs);
    chicken3421::delete_shader(vs);
    chicken3421::delete_shader(fs);

    GLint p_loc = chicken3421::get_uniform_location(render_program, "projection");
    GLint v_loc = chicken3421::get_uniform_location(render_program, "view");
    GLint m_loc = chicken3421::get_uniform_location(render_program, "model");
    //GLint color = chicken3421::get_uniform_location(render_program, "color");

    return renderer_cube{projection, render_program, p_loc, v_loc, m_loc};
}

void bind(const mesh_t mesh) {
    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glDrawArrays(GL_TRIANGLES, 0, mesh.nverts);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void draw(const renderer_t &renderer,  const glm::mat4 &p, const glm::mat4 &v, const glm::mat4 &m1, const model_t &m) {
    for (const mesh_t &mesh: m.meshes) {

        if (mesh.material_id != -1) {
            const material_t &mtl = m.materials[mesh.material_id];
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mtl.texture);
        } 

        glUniformMatrix4fv(renderer.p_loc, 1, GL_FALSE, glm::value_ptr(p));
        glUniformMatrix4fv(renderer.v_loc, 1, GL_FALSE, glm::value_ptr(v));
        glUniformMatrix4fv(renderer.m_loc, 1, GL_FALSE, glm::value_ptr(m1));

        bind(mesh);
    }
}

GLuint load_shader(const std::string &path, GLenum type) {
    std::string source = chicken3421::read_file(path);
    const char* src = source.data();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    return shader;
}



void drawCube(const renderer_cube &renderer,  const glm::mat4 &p, const glm::mat4 &v, const glm::mat4 &m1, cube cube, const node_t &m) {
    for (int i = 0; i != cube.cubePositions.size(); ++i) {
        glBindVertexArray(cube.vao);
        glBindBuffer(GL_ARRAY_BUFFER, cube.vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.ibo);
        glm::mat4 xform = glm::translate(cube.cubePositions[i], {0.0f, m.rotation.y, 0.0f});
        glUniformMatrix4fv(renderer.p_loc, 1, GL_FALSE, glm::value_ptr(p));
        glUniformMatrix4fv(renderer.v_loc, 1, GL_FALSE, glm::value_ptr(v));
        glUniformMatrix4fv(renderer.m_loc, 1, GL_FALSE, glm::value_ptr(xform));
        glDrawElements(GL_TRIANGLES, cube.nverts , GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void drawPrimitive(const renderer_t &renderer, const glm::mat4 &p, const glm::mat4 &v, const glm::mat4 &m1, const model_t &m ) {
    for (const mesh_t &mesh: m.meshes) {

        if (mesh.material_id != -1) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mesh.material_id);
        } 

        glBindVertexArray(mesh.vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
        
        // set the specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mesh.specular_map);

        glDrawElements(GL_TRIANGLES, (GLsizei) mesh.nverts, index_type, nullptr);
        glUniformMatrix4fv(renderer.p_loc, 1, GL_FALSE, glm::value_ptr(p));
        glUniformMatrix4fv(renderer.v_loc, 1, GL_FALSE, glm::value_ptr(v));
        glUniformMatrix4fv(renderer.m_loc, 1, GL_FALSE, glm::value_ptr(m1));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}


void render(const renderer_t &renderer, const scene_t &scene, const renderer_cube &cube_renderer) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(renderer.program);
    glm::mat4 p = renderer.projection;
    glm::mat4 v = get_view(scene.cam);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glUniform3fv(renderer.light_pos_loc, 1, glm::value_ptr(scene.sunlight.dir));
    glUniform3fv(renderer.light_ambient_loc, 1, glm::value_ptr(scene.sunlight.ambient));
    glUniform3fv(renderer.light_diffuse_loc, 1, glm::value_ptr(scene.sunlight.diffuse));

    const glm::mat4 &vp = renderer.projection * get_view(scene.cam);


    // an iterative approach allows for certain optimisations like tail-recursion not used here.
    // also, recursion of arbitrary depth is impossible, so this approach is more robust.
    std::stack<std::pair<const node_t &, glm::mat4>> renderables{{{ scene.root, scene.root.transform }}};
    while (!renderables.empty()) {
        const auto &[node, parent_tf] = renderables.top();  // this is called a structured binding
        renderables.pop();

        glm::mat4 xform = parent_tf * node.transform;
        glUniform1i(renderer.specular, 1);

        if (node.kind == node_t::TRACK || node.kind == node_t::BARRIER) {
            drawPrimitive(renderer, p , v, xform, node.obj);
        } else {
            draw(renderer, p, v, xform, node.obj);
        }
   
        if (node.kind == node_t::ANGEL) {
            glUseProgram(cube_renderer.program);
            drawCube(cube_renderer, p, v, vp, node.cubes, node);
            glUseProgram(renderer.program);
        }

        for (const node_t &c: node.children) {
            renderables.push({ c, xform });
        }
    }
}