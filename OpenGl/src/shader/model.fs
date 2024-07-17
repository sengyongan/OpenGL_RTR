#version 330 core
layout(location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

//in
in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;
in mat3 TBN;
//贴图
uniform sampler2D texture_diffuse1;//texture(texture_diffuse1, v_TexCoord);
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

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

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir);
vec2 texCoords;
//
void main()
{
    // Offset texture coordinates with Parallax Mapping计算偏移量
    vec3 viewDir_Depth  = normalize(constVal.camera_Position - v_Position);
    texCoords = ParallaxMapping(v_TexCoord,  viewDir_Depth);
    //if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
    //    discard;

    //
    vec3 viewPos = normalize(constVal.camera_Position - v_Position);//视线

    //
    vec3 normal = texture(texture_normal1,texCoords).rgb;
    // Transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0); 
    vec3 norm = normalize(TBN * normal);//片段modle后的法线
    //
    vec3 result = CalcDirLight(dirLight, norm, viewPos);

    for(int i = 0; i < 10; i++)
        result += CalcPointLight(pointLights[i], norm, v_Position, viewPos);

    result += CalcSpotLight(spotLight, norm, v_Position, viewPos);   
    

    //反射
    //vec3 I = normalize(v_Position - constVal.camera_Position);
    //vec3 R = reflect(I, norm);
    //FragColor = vec4(texture(skybox, R).rgb, 1.0);

    //折射
    //float ratio = 1.00 / 1.52;
    //vec3 I = normalize(v_Position - constVal.camera_Position);
    //vec3 R = refract(I, norm, ratio);
    //FragColor = vec4(texture(skybox, R).rgb, 1.0);
	BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

    FragColor = vec4(result, 1.0);//最终片段颜色
}
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    // number of depth layers
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy * 0.1f; 
    vec2 deltaTexCoords = P / numLayers;

    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(texture_height1, currentTexCoords).r;

    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(texture_height1, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }

     // -- parallax occlusion mapping interpolation from here on
    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(texture_height1, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
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
    vec3 ambient = light.ambient * texture(texture_diffuse1, texCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, texCoords).rgb;
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, texCoords));

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
    vec3 ambient = light.ambient * texture(texture_diffuse1, texCoords).rgb;
    vec3 diffuse = light.color * light.diffuse * diff * texture(texture_diffuse1, texCoords).rgb;
    vec3 specular = light.color * light.specular * spec * vec3(texture(texture_specular1, texCoords));
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
    vec3 ambient = light.ambient * texture(texture_diffuse1, texCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, texCoords).rgb;
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, texCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

