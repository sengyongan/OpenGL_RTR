#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Color;

out VS_OUT {
    vec3 color;
} vs_out;

layout (std140) uniform Matrices
{
	mat4 u_ViewProjection;
};
uniform mat4 model;

void main()
{
    vs_out.color = a_Color;
    gl_Position = u_ViewProjection * model * vec4(a_Position.x, a_Position.y, a_Position.z, 1.0); 
}