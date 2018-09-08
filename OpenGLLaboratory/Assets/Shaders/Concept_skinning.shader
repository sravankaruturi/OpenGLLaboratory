#shader vertex
#version 330 core

const int MAX_BONES = 32;

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoords;
//layout(location = 2) in uvec4 aBoneIds;
layout(location = 2) in ivec4 aBoneIds;
layout(location = 3) in vec4 aWeights;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_BoneMatrices[MAX_BONES];

out vec2 texCoords;
out vec4 colour;

void main() {

	colour = vec4(1, 1, 1, 1);

	if (aBoneIds.x > 32) {
		colour = vec4(1, 0, 0, 1);
	}
	else {
		colour = vec4(0, 1, 0, 1);
	}

	mat4 boneTransforms = u_BoneMatrices[aBoneIds[0]] * aWeights[0];
	boneTransforms += u_BoneMatrices[aBoneIds[1]] * aWeights[1];
	boneTransforms += u_BoneMatrices[aBoneIds[2]] * aWeights[2];
	boneTransforms += u_BoneMatrices[aBoneIds[3]] * aWeights[3];

	//colour = vec3(1.0);

	//if (u_BoneMatrices[aBoneIds[0]][0][0] > 0.9 ) {
	//	colour = vec3(0, 1, 0);
	//}
	//else {
	//	colour = vec3(1, 0, 0);
	//}

	vec4 positionFour = vec4(aPosition, 1.0f);
	vec4 positionLocal = boneTransforms * positionFour;

	// Use the Position Four because the Indices are messed up.
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * positionLocal;

	texCoords = aTexCoords;

}

#shader fragment
#version 330 core

in vec2 texCoords;
in vec4 colour;

uniform sampler2D u_Texture;

layout(location = 0) out vec4 outColour;

void main() {

	outColour = mix(texture(u_Texture, texCoords), colour, 0.1);

}