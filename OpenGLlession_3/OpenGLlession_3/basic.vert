#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
out vec3 Color;
struct LightInfo{
    vec4 Position;
    vec3 Intensity;
};
uniform LightInfo Lights[5];

uniform vec3 Ka;
uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ks;
uniform float Shininess;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;
vec3 ads(int lightIndex,vec4 position,vec3 norm){
    vec3 s;
    if(Lights[lightIndex].Position.w==0.0)//directional light source
        s = normalize( vec3(Lights[lightIndex].Position) );
    else
        s = normalize( vec3(Lights[lightIndex].Position - position) );
    vec3 v = normalize(vec3(-position));
    vec3 r = reflect( -s, norm );
    vec3 I = Lights[lightIndex].Intensity;
    return
    I * ( Ka +
         Kd * max( dot(s, norm), 0.0 ) +
         Ks * pow( max( dot(r,v), 0.0 ), Shininess ) );
}
void main()
{
    vec3 tnorm = normalize( NormalMatrix * VertexNormal);
    vec4 eyeCoords = ModelViewMatrix * vec4(VertexPosition,1.0);
    Color=vec3(0.0);
    for(int i=0;i<5;i++){
        Color+=ads(i,eyeCoords,tnorm);
    }
    //Color=vec3(1.0,0.0,0.0);
    gl_Position =  MVP*vec4(VertexPosition,1.0);
    //gl_Position =  vec4(VertexPosition.x+0.5,VertexPosition.y,VertexPosition.z+1,1.0);
}
