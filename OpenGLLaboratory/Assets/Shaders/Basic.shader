#shader vertex
#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTexCoord;

out vec2 texCoord;

void main() {
	gl_Position = vec4(aPosition, 0.0f, 1.0f);
	texCoord = aTexCoord;
}

#shader fragment
#version 330 core

in vec2 texCoord;
uniform sampler2D u_Texture;

layout(location = 0) out vec4 colour;

void main() {

	colour = texture(u_Texture, texCoord);

}