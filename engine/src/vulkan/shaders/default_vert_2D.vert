#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec4 frag_color;
layout(location = 1) out vec2 frag_texcoord;

layout(binding = 0) uniform Matrices {
    mat4 mvp;
} matrices;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
	frag_color = color;
    frag_texcoord = uv;
    
    gl_Position = matrices.mvp * vec4(position, 1.0);
}
