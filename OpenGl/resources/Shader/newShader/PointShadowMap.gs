#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;//渲染到6个方向

uniform mat4 shadowMatrices[6];

out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; //设置渲染到的立方体面
        for(int i = 0; i < 3; ++i) // 三角形的3个顶点
        {
            FragPos = gl_in[i].gl_Position;//世界空间坐标
            gl_Position = shadowMatrices[face] * FragPos;//渲染到6个方向
            EmitVertex();
        }    
        EndPrimitive();
    }
} 