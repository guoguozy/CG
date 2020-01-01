#version 330 core

//glVertex所指定顶点位置
layout(location = 0) in vec3 position;

//建模、观察、投影矩阵
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 eye_position; //照相机位置
uniform vec3 light_position; //光源位置
uniform float radius;

//phone shading模型所需向量
varying vec3 vertex_to_light;
varying vec3 vertex_to_eye;


void main()
{
    vec4 vertex_in_modelview_space = view*model*vec4(position,1);
    gl_Position =projection*view*model*vec4(position,1);

    //计算phone shading所需向量
    vertex_to_eye =  eye_position - vec3(vertex_in_modelview_space );
    vertex_to_light = light_position - vec3(vertex_in_modelview_space);

    //设置球体直径，近大远小
    float dist = dot(vertex_to_eye,vertex_to_eye);
    gl_PointSize = radius*2/dist;
}
