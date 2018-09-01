#shader vertex

#version 330 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;
layout(location = 3) in ivec4 BoneIDs;
layout(location = 4) in vec4 Weights;

out vec2 TexCoord0;
out vec3 Normal0;
out vec3 WorldPos0;

const int MAX_BONES = 100;

uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform mat4 u_Bones[MAX_BONES];

void main() {

	mat4 BoneTransform = u_Bones[BoneIDs[0]] * Weights[0];
	BoneTransform += u_Bones[BoneIDs[1]] * Weights[1];
	BoneTransform += u_Bones[BoneIDs[2]] * Weights[2];
	BoneTransform += u_Bones[BoneIDs[3]] * Weights[3];

	vec4 local_position = BoneTransform * vec4(Position, 1.0f);

	gl_Position = u_MVP * local_position;

	TexCoord0 = TexCoord;
	Normal0 = (u_MVP * BoneTransform * vec4(Normal, 0.0)).xyz;
	WorldPos0 = (u_MVP * local_position).xyz;

}

#shader fragment

#version 330 core
// Remove all the Light stuff. We can add them later on.

in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;

struct VSOutput
{
	vec2 TexCoord;
	vec3 Normal;
	vec3 WorldPos;
};

uniform sampler2D u_ColorMap;
uniform vec3 u_CameraWorldPos;

out vec4 FragColour;

void main() {

	VSOutput In;
	In.TexCoord = TexCoord0;
	In.Normal = normalize(Normal0);
	In.WorldPos = WorldPos0;

	FragColour = texture(u_ColorMap, In.TexCoord.xy);

}