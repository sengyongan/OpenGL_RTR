#version 330 core

layout(location = 0) out vec4 x;

uniform vec4 ourColor;

void main()
{
	x = ourColor;
}
