#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;
out vec3 positioN;
out vec3 norM;
void main()
{
    vec3 tnorm = normalize( NormalMatrix * VertexNormal);
    vec4 eyeCoords = ModelViewMatrix * vec4(VertexPosition,1.0);
    norM=tnorm;
    positioN=VertexPosition;
    //Color=vec3(1.0,0.0,0.0);
    gl_Position =  MVP*vec4(VertexPosition,1.0);
    //gl_Position =  vec4(VertexPosition.x+0.5,VertexPosition.y,VertexPosition.z+1,1.0);
}
