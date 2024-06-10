#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;

uniform vec4 ourColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	FragColor = mix(texture(texture1, v_TexCoord), texture(texture2, v_TexCoord), 0.2) * ourColor;
}
