#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 tex_coords;
layout(location = 1) in vec3 frag_color;

layout(location = 0) out vec4 out_color;

layout(binding = 1) uniform sampler2D diffuse;

void main() {
    out_color = vec4(frag_color, 1.0) * texture(diffuse, tex_coords);
}
