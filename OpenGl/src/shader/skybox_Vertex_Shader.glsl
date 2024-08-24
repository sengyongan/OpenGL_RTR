#version 330 core
layout (location = 0) in vec3 a_Position;

out vec3 v_TexCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    v_TexCoord = a_Position;
    vec4 pos = projection * view  * vec4(a_Position, 1.0);
    gl_Position = pos.xyww;
}  
//model:摄像机位置不变，天空盒没有model，和摄像机保持同一个位置，永远像摄像机被包裹在天空盒内，看到的都是天空盒
//view：可以旋转，摄像机旋转改变屏幕的方式，其实是物体旋转，所以天空盒会旋转，看起来像环顾四周
//project:被摄像机的投影矩阵影响，投影矩阵就像摄像机的调焦距等设置，一般情况摄像机这个属性不变，但也需要及时被影响