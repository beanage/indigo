#version 330

in vec2 frag_uv;
in vec3 frag_vert;

out vec4 color;

uniform sampler2D tex;

void main()
{
	color = texture(tex, frag_uv);

	// color = vec4(1.0 * frag_vert.x, 1.0 * frag_vert.y, 1.0 * frag_vert.z, 1.0);
	// color = vec4(1.0);
}
