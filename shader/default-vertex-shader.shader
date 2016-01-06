#version 330

layout (location = 0) in vec3 vert;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 norm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 frag_uv;
out vec3 frag_vert;	
out vec3 frag_norm;

void main()
{
	gl_Position  = vec4(vert, 1);
	gl_Position *= model;
	gl_Position *= view;
	gl_Position *= projection;

	frag_vert = vec3(vec4(vert, 1) * model);
	frag_uv = uv;
	frag_norm = norm;
}
