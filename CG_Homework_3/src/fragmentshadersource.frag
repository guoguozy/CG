#version 330 core
out vec4 FragColor;

varying  vec3 vertex_to_light;
varying vec3 vertex_to_eye;
uniform float a;

void main()
{
    //将超出圆的片段丢弃
    vec2 temp = gl_PointCoord*2 - vec2(1);
    float r = dot(temp,temp);
    if (r>1.0 ) discard;

    //模拟球体法向量，计算当前片段的法向量
    vec3 FragNormal = vec3(temp,sqrt(1-r));

    //计算片段的phone着色
    const vec4 AmbientColor = vec4(0.1,0.1,2.0,0.1);
    const vec4 DiffuseColor = vec4(0.0,1.0,0.0,1.0);
    const vec4 SpecularColor = vec4(1.0,1.0,1.0,0.0);

    vec3 normalized_vertex_to_light= normalize(vertex_to_light);
    float DiffuseTerm = clamp(dot(FragNormal, normalized_vertex_to_light), 0.0, 1.0);

    //sepcular
    vec3 half_angle_vertex = normalize(vertex_to_eye+vertex_to_light);
    float SpecularTerm =  max(pow(dot(half_angle_vertex,FragNormal),a),0.0);

    FragColor = AmbientColor+DiffuseTerm*DiffuseColor+SpecularTerm*SpecularColor;
}
