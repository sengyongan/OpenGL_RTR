#version 330 core

layout(location = 0) out vec4 FragColor;

in vec3 v_Position;
in vec2 v_TexCoord;
in vec3 v_Normal;

uniform vec4 ourColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 viewPos; 

vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);
vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

void main()
{
    //diffuse 
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(lightPos - v_Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - v_Position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    //
	FragColor = mix(texture(texture1, v_TexCoord), texture(texture2, v_TexCoord), 0.2) 
        * (vec4(diffuse , 1.0)  + vec4(specular, 1.0));
}
