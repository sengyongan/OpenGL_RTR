#version 330 core

layout(location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 model;
layout (std140) uniform Matrices
{
	mat4 u_ViewProjection;
};

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * model * vec4(a_Position, 1.0);	
}