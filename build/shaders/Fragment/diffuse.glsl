#version 150

in vec4 Color;
in vec3 Normal;
in vec3 FragPos;

out vec4 outColor;

uniform vec3 lightPos1;
uniform vec3 lightPos2;


void main() 
{
	float ambientStrength = 0.4f;
	vec3 ambient = ambientStrength * vec3(Color.r, Color.g, Color.b);

	vec3 norm = normalize(Normal);
	vec3 lightDir1 = normalize(lightPos1 - FragPos);
	vec3 lightDir2 = normalize(lightPos2 - FragPos);


	float diff1 = max(dot(norm, lightDir1), 0.0);
	float diff2 = max(dot(norm, lightDir2), 0.0);


	vec3 diffuse = (diff1 + diff2) * vec3(Color.r, Color.g, Color.b);

	vec3 result = diffuse + ambient;

	outColor = vec4(result, 1.0f) ;	
}