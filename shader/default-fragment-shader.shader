#version 330

uniform mat4 model;
uniform sampler2D tex;

uniform struct light {
	vec3 position;
	vec3 color;
} light_1;

in vec2 frag_uv;
in vec3 frag_vert;
in vec3 frag_norm;

out vec4 color;

void main()
{
	mat3 normal_matrix = transpose(mat3(model));
	vec3 norm = normalize(normal_matrix * frag_norm);
	vec3 frag_pos = vec3(model * vec4(frag_vert, 1));

	vec3 surface_to_light = light_1.position - frag_pos;
	float brightness = clamp(dot(norm, surface_to_light) / (length(surface_to_light) * length(norm)), 0, 1);
	
	vec4 tex_color = texture(tex, frag_uv);
	color = vec4(brightness * light_1.color * tex_color.rgb, tex_color.a);
}
