#version 460 core

layout ( location = 0 ) in vec3 vPosition;
layout ( location = 1 ) in vec2 texCoord;
layout ( location = 2 ) in vec3 normal;


out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	TexCoord = texCoord;
	gl_Position = projection * view * model * vec4(vPosition, 1.0);
}