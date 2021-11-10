#version 330 core

uniform mat4 p;
uniform mat4 v;
uniform mat4 m;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 nrm_in;

out vec2 vTexCoord;
out vec3 frag_pos;
out vec3 nrm;

void main() {
    nrm = mat3(transpose(inverse(m))) * nrm_in;
    frag_pos = vec3(m * vec4(aPos, 1.0));
    gl_Position = p * v * m * vec4(aPos, 1);
    vTexCoord = aTexCoord;
}