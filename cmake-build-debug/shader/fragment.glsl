#version 330 core

uniform sampler2D sampler;

in vec3 fragNormal;
in vec3 fragPos;
in vec2 fragTexPos;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
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
    vec3 ambTerm = light.ambient * material.ambient;

    vec3 normal = normalize(fragNormal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffTerm = light.diffuse * material.diffuse * diff;

	vec3 viewDir = normalize(camera - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specTerm = light.specular * material.specular * spec;

	vec4 texture = texture(sampler, fragTexPos);
	gl_FragColor = vec4(texture.rgb * (ambTerm + diffTerm + specTerm), texture.a);
}