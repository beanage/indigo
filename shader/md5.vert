#version 330

layout (location = 0) in vec2 uv;
layout (location = 1) in float start_weight;
layout (location = 2) in float count_weight;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 bones[64];

uniform samplerBuffer weight_tex;

//out vec2 frag_uv;
//out vec3 frag_vert;	
//out vec3 frag_norm;
out vec4 frag_color;

struct weight
{
	int bone;
	float bias;
	vec3 position;
};

weight get_weight(int i)
{
	weight w;
	w.bone = int(texelFetch(weight_tex, i*5).r);
	w.bias = texelFetch(weight_tex, i*5 + 1).r;
	w.position = vec3(texelFetch(weight_tex, i*5 + 2).r, texelFetch(weight_tex, i*5 + 3).r, texelFetch(weight_tex, i*5 + 4).r);

	return w;
}

void main()
{
	frag_color = vec4(1, 1, 1, 1);

	vec4 final_vert = vec4(0, 0, 0, 1);
	float bias_sum = 0.0;
	for (int i = 0; i < count_weight; ++i) {
		weight w = get_weight(int(start_weight) + i);
		mat4 b = bones[w.bone];
		
		final_vert += vec4(w.position, 1) * b * w.bias;
		bias_sum += w.bias;
	}

	if (bias_sum > 1.0)
		frag_color = vec4(1, 0, 0, 1);
	if (bias_sum < 1.0)
		frag_color = vec4(0, 1, 0, 1);

	gl_Position  = final_vert;
	gl_Position *= model;
	gl_Position *= view;
	gl_Position *= projection;
}
