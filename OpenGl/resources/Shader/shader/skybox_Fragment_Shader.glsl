#version 330 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

in vec3 v_TexCoord;

uniform samplerCube skybox;

void main()
{   
    BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    FragColor = texture(skybox, v_TexCoord);

}