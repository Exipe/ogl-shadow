#version 330 core

in vec3 fragNormal;
in vec3 fragPos;
in vec2 fragTexPos;

struct Material
{
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 camera;
uniform Material material;
uniform Light light;

void main() {
	vec3 matDiffuse = texture(material.diffuse, fragTexPos).rgb;
    vec3 ambTerm = light.ambient * matDiffuse;

    vec3 normal = normalize(fragNormal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffTerm = light.diffuse * matDiffuse * diff;

	vec3 viewDir = normalize(camera - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specTerm = light.specular * material.specular * spec;

	gl_FragColor = vec4(ambTerm + diffTerm + specTerm, 1.0);
}