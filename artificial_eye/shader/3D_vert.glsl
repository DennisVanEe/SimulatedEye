#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 textC;

out vec2 textCoord;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main()
{
	// just update the position
	gl_Position = projection * view * model * vec4(pos, 1.0f);
	textCoord = vec2(textC.x, textC.y); //textCoord;
}