#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 model;
layout (std140) uniform Matrices
{
	mat4 u_ViewProjection;
};
void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(u_ViewProjection * model)));
    vs_out.normal = vec3(vec4(normalMatrix * a_Normal, 0.0));
    gl_Position =u_ViewProjection * model * vec4(a_Position, 1.0); 
}