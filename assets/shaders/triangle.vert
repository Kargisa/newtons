#version 450

vec2 positions[3] = vec2[](
	vec2(0.5, 0.5),
	vec2(-0.5, 0.5),
	vec2(0.0, -0.5)
);

layout(location = 0) in vec3 inPosition;

vec3 colors[3] = vec3[](
	vec3(1, 1, 0),
	vec3(0, 1, 1),
	vec3(1, 0, 1)
);

layout(location = 0) out vec3 vertColor;

void main() {
    gl_Position = vec4(inPosition, 1.0);
	vertColor = colors[gl_VertexIndex];
}