#version 330 core

layout(location = 0) out vec4 FragColor;

in vec3 v_Position;
in vec2 v_TexCoord;
in vec3 v_Normal;

uniform vec4 ourColor;

//diffuse
uniform sampler2D texture1;
uniform sampler2D texture2;
//specular
uniform sampler2D specular_Texture;
//
uniform vec3 viewPos; 
//
vec3 ambient = vec3(0.1f, 0.1f, 0.1f);
vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);
vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
//
struct Light{

    //Attenuation
    float constant;
    float linear;
    float quadratic;

    //Flashlight
    vec3 camera_Position;  
    vec3 camera_Direction;
    float cutOff;
    float outerCutOff;
};
uniform Light light;




void main()
{
    vec3 lightDir = normalize(light.camera_Position - v_Position);
    

    //diffuse 
    vec3 norm = normalize(v_Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * mix(texture(texture1, v_TexCoord), texture(texture2, v_TexCoord), 0.0).rgb;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - v_Position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = specularStrength * spec * lightColor * vec3(texture(specular_Texture, v_TexCoord));  
    
    //
    float theta = dot(lightDir, normalize(-light.camera_Direction)); 
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);    
    diffuse  *= intensity;
    specular *= intensity;

    // attenuation
    float distance    = length(lightPos - v_Position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    diffuse  *= attenuation;
    specular *= attenuation ;   

    //
    vec3  result = diffuse + specular;
	FragColor = vec4(result , 1.0) ;

}
