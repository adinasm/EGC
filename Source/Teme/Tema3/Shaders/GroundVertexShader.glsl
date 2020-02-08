#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec2 heightMapSize;

uniform float height_factor;

uniform sampler2D heightMap;

out vec2 texcoord;
out vec3 normal;
out vec3 world_position;
out vec3 world_normal;

void main()
{
	// Pass v_texture_coord as output to Fragment Shader
	texcoord = v_texture_coord;

	float height = height_factor * texture2D(heightMap, vec2(v_texture_coord.x, v_texture_coord.y)).r;
	vec2 texelSize = vec2(1.f / heightMapSize.x, 1.f / heightMapSize.y);
	float heightRight = height_factor * texture2D(heightMap, vec2(v_texture_coord.x + texelSize.x, v_texture_coord.y)).r;
	float heightUp = height_factor * texture2D(heightMap, vec2(v_texture_coord.x, v_texture_coord.y + texelSize.y)).r;

	float Hx = height - heightRight;
	float Hz = height - heightUp;

	normal = normalize(vec3(Hx * 20.f, 1.f, Hz * 20.f));
	vec4 pos = vec4(v_position.x, height, v_position.z, 1.f);

	world_position = (Model * pos).xyz;
	world_normal = normalize(mat3(Model) * normal);

	gl_Position = Projection * View * Model * pos;
}
