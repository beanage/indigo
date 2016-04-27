#version 330

layout (location = 0) in vec3 vert;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec2 cell_position;
uniform vec2 cell_count;

uniform float cell_size;
uniform float tile_size;

out vec2 vert_uv;

void main()
{
    vert_uv = uv;
     
    vec3 cell_vert = vert;
    cell_vert += vec3((cell_position - cell_count / 2.0) * tile_size * cell_size, 0);
     
	gl_Position  = vec4(cell_vert, 1);
	gl_Position *= model;
	gl_Position *= view;
	gl_Position *= projection;
}
