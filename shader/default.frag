#version 330

uniform mat4 model;
uniform sampler2D tex;

uniform struct light {
    vec3 direction;
    vec3 color;
} sun;

in vec2 frag_uv;
in vec3 frag_vert;
in vec3 frag_norm;

out vec4 color;

void main()
{
    const float min_light = 0.15;
    
    mat3 normal_matrix = transpose(mat3(model));
    vec3 norm = normalize(normal_matrix * frag_norm);
    vec3 frag_pos = vec3(model * vec4(frag_vert, 1));

    // Directional light
    vec3 surface_to_light = normalize(sun.direction);
    
    // Point light
    // vec3 surface_to_light = sun.direction - frag_pos;

    float brightness = clamp(dot(norm, surface_to_light) / (length(surface_to_light) * length(norm)), min_light, 1);
    
    // vec4 tex_color = texture(tex, frag_uv);
    vec4 tex_color = vec4(.3,.4,.5,1.);
    color = vec4(brightness * sun.color * tex_color.rgb, tex_color.a);
}
