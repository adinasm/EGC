#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// Output values to fragment shader
out vec3 world_position;
out vec3 world_normal;

void main()
{
	float pi = 3.14159;
	float dist = length(v_position);
	float dx = cos(2 * pi * dist + Time);
	float dy = 0.3f * sin(23 * pi * dist + 2 * Time);
	float dz = sin(2 * pi * dist + Time);

	vec3 pos = vec3(v_position.x + dx, v_position.y + dy, v_position.z + dz);

	// Compute world space vertex position and normal
	// Send world position and world normal to Fragment Shader
	world_position = (Model * vec4(pos, 1)).xyz;
	world_normal = normalize(mat3(Model) * v_normal);

	gl_Position = Projection * View * Model * vec4(pos, 1.0);
}
