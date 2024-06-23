#version 330 core
layout (location = 0) in vec3 a_Position;

uniform mat4 model;
layout (std140) uniform Matrices
{
	mat4 u_ViewProjection;
};

void main()
{
    gl_Position = u_ViewProjection * model * vec4(a_Position, 1.0);
    gl_PointSize = gl_Position.z;    
}