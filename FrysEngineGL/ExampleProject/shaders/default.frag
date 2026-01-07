#version 330 core

out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;
uniform Light light;
uniform vec3 viewPosition;

void main()
{
	vec3 lightDir = normalize(light.position - FragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	float dist = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	// Calculate ambient light
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	vec3 normal = normalize(Normal);

	// Calculate diffuse
	float diffuseStrength = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diffuseStrength * texture(material.diffuse, TexCoords).rgb;

	// calculate specular
	vec3 viewDir = normalize(viewPosition - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	// use attenuation
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// we'll leave ambient unaffected so we always have a little light.
	diffuse *= intensity;
	specular *= intensity;

	// combine
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}