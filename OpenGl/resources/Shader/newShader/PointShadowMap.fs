#version 330 core
in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
    float lightDistance = length(FragPos.xyz - lightPos);//计算片段深度值
    
    // 转为 [0;1] 之间
    lightDistance = lightDistance / far_plane;
    
    // 写入深度
    gl_FragDepth = lightDistance;
}