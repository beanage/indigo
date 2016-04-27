#version 330

uniform vec3 cursor;

uniform sampler2D blend;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
//uniform sampler2D tex3;

in vec2 vert_uv;
out vec4 color;

void main()
{
    vec4 blending = texture(blend, vert_uv);
    float total_blending = blending.r + blending.g + blending.b + blending.a;
    
    vec3 color0 = texture(tex0, vert_uv).rgb;
	vec3 color1 = texture(tex1, vert_uv).rgb;
	vec3 color2 = texture(tex2, vert_uv).rgb;
	//vec3 color3 = texture(tex3, vert_uv).rgb;

    color = vec4(
        color0 * (blending.r / total_blending) + 
        color1 * (blending.g / total_blending) +
        color2 * (blending.b / total_blending), 1);
}
