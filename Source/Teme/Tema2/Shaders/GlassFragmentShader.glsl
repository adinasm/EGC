#version 330

in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform float transparency;

uniform vec3 headlight_direction;
uniform vec3 headlight_position;

uniform float cut_off;

uniform vec3 object_color;

layout(location = 0) out vec4 out_color;

void main()
{
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);
	vec3 R = normalize(reflect(L, normalize(world_normal)));

	vec3 LS = normalize(headlight_position - world_position);
	vec3 VS = normalize(eye_position - world_position);
	vec3 HS = normalize(LS + VS);
	vec3 RS = normalize(reflect(LS, normalize(world_normal)));

	// Define ambient light component
	float ambient_light = 0.25;
	float cloud_shininess = 20;

	// Compute diffuse light component
	float diffuse_light = 0.2f * max(dot(normalize(world_normal), L), cloud_shininess);

	// Compute specular light component
	float specular_light = 0;

	if (diffuse_light > 0)
	{
		// specular_light = material_ks * pow(max(dot(V, R), 0), cloud_shininess);
		specular_light = 0.8f * pow(max(dot(normalize(world_normal), H), 0), cloud_shininess);
	}

	// Compute diffuse light component
	float head_diffuse_light = material_kd * max(dot(normalize(world_normal), LS), 0.f);

	// Compute specular light component
	float head_specular_light = 0.f;

	if (head_diffuse_light > 0.f)
	{
		// head_specular_light = material_ks * pow(max(dot(VS, RS), 0.f), material_shininess);
		head_specular_light = material_ks * pow(max(dot(normalize(world_normal), HS), 0.f), material_shininess);
	}

	// Compute light
	float head_light = 0.f;

	float spot_light = dot(-LS, headlight_direction);
	float spot_light_limit = cos(cut_off);
	float light_att_factor = 0.f;
 
	if (spot_light > cos(cut_off))
	{
		// Quadratic attenuation
		float linear_att = (spot_light - spot_light_limit) / (1.f - spot_light_limit);
		light_att_factor = 1.f / max(pow(linear_att, 2), 1.f);
	}

	head_light = light_att_factor * (head_diffuse_light + head_specular_light);

	float d = distance(light_position, world_position);
	float attenuation_factor = 1.f / max(1.f + 0.0014f * d + 0.000007 * pow(d, 2), 1.f);
	float light = head_light + ambient_light + attenuation_factor * (diffuse_light + specular_light);

	out_color = vec4(light * object_color, transparency);
}