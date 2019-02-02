#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform Matrices {
    mat4 view;
    mat4 projection;
    mat4 pv;
} matrices;

layout(location = 0) in vec3 position;
layout(location = 0) out vec3 frag_color;

layout(binding = 1) uniform Model {
    mat4 matrix;
} model;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {

vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

    gl_Position = matrices.pv * model.matrix * vec4(position, 1.f);
    frag_color = colors[gl_VertexIndex];
}
