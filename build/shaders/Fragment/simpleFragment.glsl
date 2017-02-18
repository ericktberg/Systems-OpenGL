#version 150

in vec4 Color;
in vec3 Normal;
in vec3 FragPos;

out vec4 outColor;

uniform vec3 lightPos1;
uniform vec3 lightPos2;

void main() 
{
	outColor = Color;	
}