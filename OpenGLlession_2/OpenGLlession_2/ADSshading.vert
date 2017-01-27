#version 400
layout(location=0)in vec3 VertexPosition;
layout(location=1)in vec3 VertexNormal;
flat out vec3 FrontColor;
flat out vec3 BackColor;
struct LightInfo {
    vec4 Position;//在眼睛的视角中光的位置
    vec3 La;//环境光强度
    vec3 Ld;//漫反射光强度
    vec3 Ls;//镜面反射光强度
};
uniform LightInfo Light;
struct MaterialInfo{
    vec3 Ka;//漫反射材质系数
    vec3 Kd;
    vec3 Ks;
    float Shininess;
};
uniform MaterialInfo Material;
uniform mat4 ModelViewMaterix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;
subroutine vec3 shadeModelType(vec4 position,vec3 normal);
subroutine uniform shadeModelType shadelModel;
subroutine(shadeModelType)
vec3 phongModel(vec4 position,vec3 normal){
    vec3 s=normalize(vec3(Light.Position-position));
    vec3 v=normalize(-position.xyz);
    vec3 r=reflect(-s,normal);
    vec3 ambient=Light.La*Material.Ka;
    float sDotN=max(dot(s,normal),0.0);
    vec3 diffuse=Light.Ld*Material.Kd*sDotN;
    vec3 spec=vec3(0.0);
    if(sDotN>0.0){
        spec=Light.Ls*Material.Ks*pow(max(dot(r,v),0.0),Material.Shininess);
    }
    vec3 LightIntensity=diffuse+spec+ambient;
    return LightIntensity;
}
subroutine(shadeModelType)
vec3 diffuseOnly(vec4 position,vec3 normal){
    vec3 s=normalize(vec3(Light.Position-position));
    return Light.Ld*Material.Kd*max(dot(s,normal),0.0);
}
void main()
{
    vec3 tnorm=normalize(NormalMatrix*VertexNormal);//将其转化为观察坐标系的法向量
    vec4 eyeCoords=ModelViewMaterix*vec4(VertexPosition,1.0f);//观察视角的位置
    FrontColor=shadelModel(eyeCoords,tnorm);
    BackColor=shadelModel(eyeCoords,-tnorm);
    gl_Position=MVP*vec4(VertexPosition,1.0);
}
