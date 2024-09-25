#version 440 core
layout(location = 0) in vec3 vertexPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



out vec3 vsPosition;

void main()
{
	vsPosition = vertexPosition;
	gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
}
