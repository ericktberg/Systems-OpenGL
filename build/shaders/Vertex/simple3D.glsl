#version 150

in vec4 color;
in vec3 position;
in vec3 normal;


uniform mat4 model;
uniform mat4 camera;

out vec3 FragPos;
out vec4 Color;
out vec3 Normal;

void main() 
{
	Color = color;
	Normal = mat3(transpose(inverse(model))) * normal;
	gl_Position = camera * model * vec4(position, 1.0);
	FragPos = vec3(model * vec4(position, 1.0f));
}