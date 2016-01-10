#version 330

uniform mat4 model;
//uniform sampler2D tex;

//in vec2 frag_uv;
//in vec3 frag_vert;
//in vec3 frag_norm;
in vec4 frag_color;

out vec4 color;

void main()
{
	color = frag_color; // vec4(1, 1, 1, 1); //texture(tex, frag_uv);
}
