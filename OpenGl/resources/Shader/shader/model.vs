#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;


out vec3 v_Position;
out vec2 v_TexCoord;
out vec3 v_Normal;
out mat3 TBN;

uniform mat4 model;
layout (std140) uniform Matrices
{
	mat4 u_ViewProjection;
};

void main()
{

	vec3 T = normalize(vec3(model * vec4(tangent,   0.0)));
	vec3 N = normalize(vec3(model * vec4(a_Normal,    0.0)));
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(T, N);
	TBN = mat3(T, B, N);

	v_TexCoord = a_TexCoord;
	v_Position = vec3( model * vec4(a_Position, 1.0));
    v_Normal = mat3(transpose(inverse( model))) * a_Normal;  
    gl_Position = u_ViewProjection * model * vec4(a_Position, 1.0);
}