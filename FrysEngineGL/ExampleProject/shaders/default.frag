#version 330 core

out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Attenuation {
	float constant;
	float linear;
	float quadratic;
};

struct DirectionalLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	Attenuation attenuation;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	Attenuation attenuation;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;
uniform vec3 viewPosition;

uniform DirectionalLight directionalLight;

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform SpotLight spotLight;

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularColor);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 FragPos, vec3 diffuseColor, vec3 specularColor);

void main()
{
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPosition - FragPos);
	vec3 diffuseColor = vec3(texture(material.diffuse, TexCoords));
	vec3 specularColor = vec3(texture(material.specular, TexCoords));

	// phase 1: directional
	vec3 result = CalculateDirectionalLight(directionalLight, normal, viewDir, diffuseColor, specularColor);

	// phase 2: point lights
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += CalculatePointLight(pointLights[i], normal, FragPos, viewDir, diffuseColor, specularColor);
	}

	// phase 3: Spot light
	result += CalculateSpotLight(spotLight, normal, viewDir, FragPos, diffuseColor, specularColor);

	FragColor = vec4(result, 1.0);
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularColor)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// combine results
	vec3 ambient  = light.ambient * diffuseColor;
	vec3 diffuse  = light.diffuse * diff * diffuseColor;
	vec3 specular = light.specular * spec * specularColor;
	return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor)
{
	vec3 lightDir = normalize(light.position - fragPos);

	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// attenuation
	float dist = length(light.position - fragPos);
	float attenuationFactor =
		1.0 / (light.attenuation.constant + light.attenuation.linear * dist
		+ light.attenuation.quadratic * (dist * dist));

	// combine results
	vec3 ambient  = light.ambient  * diffuseColor;
	vec3 diffuse  = light.diffuse  * diff * diffuseColor;
	vec3 specular = light.specular * spec * specularColor;

	ambient  *= attenuationFactor;
	diffuse  *= attenuationFactor;
	specular *= attenuationFactor;
	return (ambient + diffuse + specular);
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 FragPos, vec3 diffuseColor, vec3 specularColor)
{
	vec3 lightDir = normalize(light.position - FragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	float dist = length(light.position - FragPos);
	float attenuationFactor =
		1.0 / (light.attenuation.constant + light.attenuation.linear * dist
		+ light.attenuation.quadratic * (dist * dist));

	// Calculate diffuse
	float diffuseStrength = max(dot(normal, lightDir), 0.0);

	// calculate specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float specularStrength = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * diffuseColor;
	vec3 diffuse = light.diffuse * diffuseStrength * diffuseColor;
	vec3 specular = light.specular * specularStrength * specularColor;

	ambient *= attenuationFactor;
	diffuse *= attenuationFactor;
	specular *= attenuationFactor;

	// we'll leave ambient unaffected so we always have a little light.
	diffuse *= intensity;
	specular *= intensity;

	// combine
	return (ambient + diffuse + specular);
}