#version 150

//uniform vec3 globalColor;

in vec4 Color;

out vec4 outColor;

void main() 
{
	outColor = vec4(Color);	
}