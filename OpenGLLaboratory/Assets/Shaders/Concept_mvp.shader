#shader vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoords;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

out vec2 texCoords;

void main() {

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(aPosition, 1.0f);
	texCoords = aTexCoords;

}

#shader fragment
#version 330 core

in vec2 texCoords;
uniform sampler2D u_Texture;

layout(location = 0) out vec4 outColour;

void main() {

	outColour = texture(u_Texture, texCoords);

}