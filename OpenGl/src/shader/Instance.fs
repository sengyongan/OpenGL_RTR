#version 330 core
layout(location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

//in
in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;

//贴图
uniform sampler2D texture_diffuse1;//texture(texture_diffuse1, v_TexCoord);
//specular贴图
uniform sampler2D texture_specular1;

uniform samplerCube skybox;

//光泽度
uniform float shininess;

//平行光
struct DirLight {
    vec3 direction;//位置

	//平行光对3中效果的强度影响
    vec3 ambient;//环境光反射
    vec3 diffuse;//漫反射
    vec3 specular;//镜面反射
};
//点光
struct PointLight {
    vec3 position;
    vec3 color;
	
    //点光对3中效果的强度影响
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
//聚光
struct SpotLight {
    //聚光范围
    float cutOff;
    float outerCutOff;

    //聚光对3中效果的强度影响
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};
struct ConstVal{
    //camera
    vec3 camera_Position;
    vec3 camera_Direction;
    //边缘软化Attenuation
    float constant;
    float linear;
    float quadratic;
};
//
uniform DirLight dirLight;
uniform PointLight pointLights[10];
uniform SpotLight spotLight;
uniform ConstVal constVal;

// function prototypes声明在此处，main可以用
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

//
void main()
{
    vec3 viewPos = normalize(constVal.camera_Position - v_Position);//视线
    vec3 norm = normalize(v_Normal);//片段modle后的法线

    vec3 result = CalcDirLight(dirLight, norm, viewPos);

    for(int i = 0; i < 10; i++)
        result += CalcPointLight(pointLights[i], norm, v_Position, viewPos);

    result += CalcSpotLight(spotLight, norm, v_Position, viewPos);   
    

    //反射
    //vec3 I = normalize(v_Position - constVal.camera_Position);
    //vec3 R = reflect(I, norm);
    //FragColor = vec4(texture(skybox, R).rgb, 1.0);

    //折射
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(v_Position - constVal.camera_Position);
    vec3 R = refract(I, norm, ratio);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
    	 BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

    //FragColor = vec4(result, 1.0);//最终片段颜色
}

//
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);//平行光向量   
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);//漫反射
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    //计算影响
    vec3 ambient = light.ambient * texture(texture_diffuse1, v_TexCoord).rgb;
    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, v_TexCoord).rgb;
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, v_TexCoord));

    return (ambient + diffuse + specular);
    }
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);//当前点光向量 
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (constVal.constant + constVal.linear * distance + constVal.quadratic * (distance * distance));    
    //计算影响
    vec3 ambient = light.ambient * texture(texture_diffuse1, v_TexCoord).rgb;
    vec3 diffuse = light.color * light.diffuse * diff * texture(texture_diffuse1, v_TexCoord).rgb;
    vec3 specular = light.color * light.specular * spec * vec3(texture(texture_specular1, v_TexCoord));
    ambient *= attenuation;
    diffuse *= attenuation ;
    specular *= attenuation ;
    return (ambient + diffuse + specular);
}
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(constVal.camera_Position - fragPos);//摄像机向量
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(constVal.camera_Position - fragPos);
    float attenuation = 1.0 / (constVal.constant + constVal.linear * distance + constVal.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-constVal.camera_Direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    //计算影响
    vec3 ambient = light.ambient * texture(texture_diffuse1, v_TexCoord).rgb;
    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, v_TexCoord).rgb;
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, v_TexCoord));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
