#shader vertex
#version 330 core

const int MAX_BONES = 100;

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in ivec4 aBoneIds;
layout(location = 3) in vec4 aWeights;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_BoneMatrices[MAX_BONES];

out vec2 texCoords;
out vec4 colour;

void main() {

	//if (aBoneIds.x > 32 || aBoneIds.y > 32 || aBoneIds.z > 32 || aBoneIds.w > 32) {
	//	colour = vec4(1, 0, 0, 1);
	//}
	//else {
	//	colour = vec4(0, 1, 0, 1);
	//}

	colour = vec4(0, 0, 0, 1);

	// The Higher the Bone Ids, the lower the colour would be., The Blacker the output would be.
	// What we want: Lower BoneIds. and Bluer Picture.
	colour.r = 1 / (aBoneIds[0] + 0.01);
	colour.g = 1 / (aBoneIds[1] + 0.01);
	colour.b = 1 / (aBoneIds[2] + 0.01);

	// The Output that we get not only mean that the Lamp is bad. But also, other stuff, like the pelvis

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

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * positionFour;

	texCoords = aTexCoords;

}

#shader fragment
#version 330 core

in vec2 texCoords;
in vec4 colour;

uniform sampler2D u_Texture;

layout(location = 0) out vec4 outColour;

void main() {

	outColour = mix(texture(u_Texture, texCoords), colour, 0.9);

}