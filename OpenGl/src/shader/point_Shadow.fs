#version 330 core

layout(location = 0) out vec4 FragColor;
//in
in vec3 v_Position;
in vec2 v_TexCoord;
in vec3 v_Normal;


//材质
struct Material {
    //贴图
    sampler2D texture1;
    sampler2D texture2;
    //specular贴图
    sampler2D specular_Texture;
    samplerCube depthMap;
    //光泽度
    float shininess;
}; 
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
    bool blinn;//Blinn-Phong
    bool gamma;
    //
    vec3 lightPos;
    bool shadows;
    float far_plane;
};
//
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[10];
uniform SpotLight spotLight;
uniform ConstVal constVal;

// function prototypes声明在此处，main可以用
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

float closestDepth;
//Shadow
float ShadowCalculation(vec3 fragPos,vec3 viewPos)
{

   // 光到片段的方向
    vec3 fragToLight = fragPos - constVal.lightPos;
    // 从立方体深度贴图采样方向上的  深度值   
    closestDepth = texture(material.depthMap, fragToLight).r;
    //变换0到far_plane的范围
    closestDepth *= constVal.far_plane;
    // 获取片段的深度（长度）
    float currentDepth = length(fragToLight);
    //计算是否在阴影
    float bias = 0.05; 
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / constVal.far_plane)) / 25.0;
    //float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
    float shadow = 0.0;
    float samples = 20;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(material.depthMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= constVal.far_plane;   // Undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }

    shadow /= float(samples);


    return shadow;
}


////2Linear
float near = 0.1; 
float far  = 1000.0; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}


//
void main()
{

    vec3 viewPos = normalize(constVal.camera_Position - v_Position);//视线
    vec3 norm = normalize(v_Normal);//片段modle后的法线

    vec3 result1 = CalcDirLight(dirLight, norm, viewPos);

    vec3 result2;
    for(int i = 0; i < 10; i++){
        result2 += CalcPointLight(pointLights[i], norm, v_Position, viewPos);
    }
    vec3 result3 = CalcSpotLight(spotLight, norm, v_Position, viewPos);  

    //Shadow
    //vec3 lightDir = normalize(constVal.lightPos - v_Position);
    //float bias = max(0.02 * (1.0 - dot(norm, lightDir)), 0.005);

    //float bias = 0.005;
    float shadow = constVal.shadows ? ShadowCalculation(v_Position,viewPos) : 0.0;     
    vec3 result = (result1 + (1.0 - shadow) * (result2 + result3));
    
    
    //FragColor = vec4(vec3(closestDepth / constVal.far_plane), 1.0);
    FragColor = vec4(result, 1.0) * vec4(0.8,0.9,1.0,1.0);//最终片段颜色 * 偏蓝色光
      
}

//
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{


    vec3 lightDir = normalize(-light.direction);//平行光向量   
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);//漫反射
    // specular shading
    float spec = 0.0;
    if(constVal.blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }

    //计算影响
    vec3 ambient = light.ambient * texture(material.texture1, v_TexCoord).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.texture1, v_TexCoord).rgb;
    vec3 specular = light.specular * spec * vec3(texture(material.specular_Texture, v_TexCoord));

    return (ambient + diffuse + specular);
    }
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);//当前点光向量 
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float spec = 0.0;
    if(constVal.blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }

    // attenuation衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (constVal.constant + constVal.linear * distance + constVal.quadratic * (distance * distance));   
    //float attenuation = 1.0 / (constVal.gamma ? distance * distance : distance);
    //计算影响
    vec3 ambient = light.ambient * texture(material.texture1, v_TexCoord).rgb;
    vec3 diffuse = light.color * light.diffuse * diff * texture(material.texture1, v_TexCoord).rgb;
    vec3 specular = light.color * light.specular * spec * vec3(texture(material.specular_Texture, v_TexCoord));
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
    float spec = 0.0;
    if(constVal.blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }

    // attenuation
    float distance = length(constVal.camera_Position - fragPos);
    float attenuation = 1.0 / (constVal.constant + constVal.linear * distance + constVal.quadratic * (distance * distance));    
    //float attenuation = 1.0 / (constVal.gamma ? distance * distance : distance);
    // spotlight intensity
    float theta = dot(lightDir, normalize(-constVal.camera_Direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    //计算影响
    vec3 ambient = light.ambient * texture(material.texture1, v_TexCoord).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.texture1, v_TexCoord).rgb;
    vec3 specular = light.specular * spec * vec3(texture(material.specular_Texture, v_TexCoord));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
