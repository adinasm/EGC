#version 330

uniform sampler2D texture;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

in vec2 texcoord;
in vec3 world_position;
in vec3 world_normal;
in vec3 normal;

layout(location = 0) out vec4 out_color;

void main()
{
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);
	vec3 R = normalize(reflect(L, normalize(world_normal)));

	// Define ambient light component
	float ambient_light = 0.25f;

	// Compute diffuse light component
	float diffuse_light = material_kd * max(dot(normalize(world_normal), L), 0.f);

	// Compute specular light component
	float specular_light = 0.f;

	if (diffuse_light > 0.f)
	{
		// specular_light = material_ks * pow(max(dot(V, R), 0.f), material_shininess);
		specular_light = material_ks * pow(max(dot(normalize(world_normal), H), 0.f), material_shininess);
	}

	float d = distance(light_position, world_position);
	float attenuation_factor = 1.f / max(1.f + 0.0014f * d + 0.000007 * pow(d, 2), 1.f);
	float light = ambient_light + attenuation_factor * (diffuse_light + specular_light);

	// Calculate the out_color using the texture2D() function
	vec4 color = texture2D(texture, texcoord);
	out_color = light * color;
}
