#version 330 core
out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D screenTexture;
uniform float near_plane;
uniform float far_plane;

uniform int screenWidth_mid;
uniform int screenHeight_mid;

const float offset = 1.0 / 300.0;  

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main()
{
    //FrameBuffer
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );

    float kernel[9] = float[](
        1, 1, 1,
        1, -8, 1,
        1, 1, 1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, v_TexCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    //ShadowMap
    float depthValue = texture(screenTexture, v_TexCoord).r;
    //FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0);

    FragColor = texture(screenTexture, v_TexCoord);

    //FrameBuffer
    //if (gl_FragCoord.x < (screenWidth_mid / 2)) {
    //    if (gl_FragCoord.y < (screenHeight_mid / 2)) {
    //        FragColor = texture(screenTexture, v_TexCoord) * vec4(1.0, 0.0, 0.0, 1.0); // 红色
    //    } else {
    //        FragColor = texture(screenTexture, v_TexCoord) * vec4(0.0, 0.0, 1.0, 1.0); // 蓝色
    //    }
    //} else {
    //    if (gl_FragCoord.y < (screenHeight_mid / 2)) {
    //        FragColor = texture(screenTexture, v_TexCoord) * vec4(0.0, 1.0, 0.0, 1.0); // 绿色
    //    } else {
    //        FragColor = texture(screenTexture, v_TexCoord) * vec4(1.0, 1.0, 0.0, 1.0); // 黄色
    //    }
    //}
}