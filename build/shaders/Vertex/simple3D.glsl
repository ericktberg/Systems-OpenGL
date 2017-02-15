#version 150

in vec4 color;
in vec3 position;

uniform mat4 model;
uniform mat4 camera;

out vec4 Color;

void main() 
{
	Color = color;
	gl_Position = camera * model * vec4(position, 1.0);
}