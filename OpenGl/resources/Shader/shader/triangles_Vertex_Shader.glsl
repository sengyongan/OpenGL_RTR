#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

out vec3 v_Position;
out vec4 v_Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
layout (std140) uniform Matrices
{
	mat4 u_ViewProjection;
};

void main()
{
	v_Position = a_Position;
	v_Color = a_Color;
	gl_Position = u_ViewProjection * model *  vec4(a_Position, 1.0);	
}
