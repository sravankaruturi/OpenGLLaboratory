#shader vertex
#version 430 core

layout(location = 0) in vec3 aPosition;

void main() {
	gl_Position = vec4(aPosition, 1.0f);
}

#shader geometry
#version 430 core

uniform mat4 u_ModelMatrix[4];

layout(triangles, invocations = 4) in;
layout(triangle_strip, max_vertices = 3) out;

void main() {
	for (int i = 0; i < gl_in.length(); i++) {
		gl_ViewportIndex = gl_InvocationID;
		gl_Position = u_ModelMatrix[gl_InvocationID] * gl_in[i].gl_Position;

		EmitVertex();
	}
}

#shader fragment
#version 430 core

layout(location = 0) out vec4 outColour;

void main() {
	vec3 g_colour = vec3(0.8, 0.3, 0.2);
	outColour = vec4(g_colour, 1.0);
}