#version 330 core
layout(location = 0) out vec4 FragColor;

in vec3 v_TexCoord;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, v_TexCoord);
}