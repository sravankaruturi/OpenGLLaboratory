#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

void main() {
	gl_Position = position;
}

#shader fragment
#version 330 core

uniform vec3 u_Colour;

layout(location = 0) out vec4 colour;

void main() {
	colour = vec4(u_Colour, 1.0f);
}