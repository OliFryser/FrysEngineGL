#version 330 core
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 ObjectColor;

uniform vec3 LightPos;
uniform vec3 LightColor;
uniform vec3 ViewPos;

void main()
{
	// Calculate ambient light
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * LightColor;

	// Calculate diffuse
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);
	float diffuseStrength = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diffuseStrength * LightColor;

	// calculate specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(ViewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * LightColor;

	// combine
	vec3 result = (ambient + diffuse + specular) * ObjectColor;
	FragColor = vec4(result, 1.0);
}