#version 330
 
uniform sampler2D texture;
 
in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
	// Calculate the out_color using the texture2D() function

	vec4 color = texture2D(texture, texcoord);         
	out_color = color;
}
