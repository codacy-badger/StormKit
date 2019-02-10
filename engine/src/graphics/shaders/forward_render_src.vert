#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

layout(location = 0) out vec4 frag_color;

layout(binding = 0) uniform Matrices {
	mat4 projection;
	mat4 view;
} matrices;

void main() {
	gl_Position = matrices.view * matrices.projection * vec4(position, 1.0);
	frag_color = color;
}
