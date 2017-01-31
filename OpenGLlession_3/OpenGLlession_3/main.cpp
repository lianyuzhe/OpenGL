//
//  main.cpp
//  OpenGLlesson_1
//
//  Created by lianyuzhe on 2017/1/25.
//  Copyright © 2017年 lianyuzhe. All rights reserved.
//
#define TINYOBJLOADER_IMPLEMENTATION
#include <GLSLProgram.hpp>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLUT/GLUT.h>
#include "tiny_obj_loader.h"
#include "define.h"
using namespace glm;
const int pig=1;
const int tour=0;
const GLuint WIDTH = 800, HEIGHT = 800;
GLFWwindow* window=nullptr;
bool ADS=true;
bool change=true;
int init();
GLint handle=0;
bool compile(GLSLProgram &proc);
bool render(GLSLProgram &proc,unsigned int &vaoHandle,unsigned int bufferHanle[3]);
bool renderPlane(unsigned int& vaoPlaneHandle,unsigned int planeBuffer[3]);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
int face=0;
int main(int argc, const char * argv[]) {
    init();
    bool success;
    GLSLProgram procPig;
    unsigned int vaoPigHandle=0,PigBuffer[3],vaoPlaneHandle,planeBuffer[3];
    success=render( procPig,vaoPigHandle,PigBuffer);
    if(!success){
        return false;
    }
    renderPlane(vaoPlaneHandle, planeBuffer);
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClearColor(0.2f,0.2f,0.2f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        procPig.use();
        procPig.setUniform("spotKd", 0.9f, 0.5f, 0.3f);
        procPig.setUniform("spotKs", 0.95f, 0.95f, 0.95f);
        procPig.setUniform("spotKa", 0.27f, 0.15f, 0.09f);
        procPig.setUniform("spotShininess", 100.0f);
        glBindVertexArray(vaoPigHandle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,PigBuffer[2]);
        glDrawElements(GL_TRIANGLES, face, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
        
        procPig.setUniform("spotKd", 0.7f, 0.7f, 0.7f);
        procPig.setUniform("spotKs", 0.9f, 0.9f, 0.9f);
        procPig.setUniform("spotKa", 0.2f, 0.2f, 0.2f);
        procPig.setUniform("spotShininess", 180.0f);
        glBindVertexArray(vaoPlaneHandle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,planeBuffer[2]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,(GLubyte*)nullptr);
        glfwSwapBuffers(window);
        glBindVertexArray(0);
    }
    return 0;
}
bool renderPlane(unsigned int& vaoPlaneHandle,unsigned int planeBuffer[3]){
    glGenVertexArrays(1,&vaoPlaneHandle);
    glBindVertexArray(vaoPlaneHandle);
    GLfloat vertices[]={
        -10.0f,0.0f, 10.0f,
         10.0f,0.0f, 10.0f,
         10.0f,0.0f,-10.0f,
        -10.0f,0.0f,-10.0f
    };
    GLfloat normal[]={
        0.0f,1.0f,0.0f,
        0.0f,1.0f,0.0f,
        0.0f,1.0f,0.0f,
        0.0f,1.0f,0.0f
    };
    GLuint element[]={
        0,1,2,
        0,2,3
    };
    glGenBuffers(3,planeBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,planeBuffer[0]);
    glBufferData(GL_ARRAY_BUFFER,4*3*sizeof(GLfloat),vertices,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)nullptr);
    glBindBuffer(GL_ARRAY_BUFFER,planeBuffer[1]);
    glBufferData(GL_ARRAY_BUFFER,4*3*sizeof(GLfloat),normal,GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,planeBuffer[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,6*sizeof(unsigned int),element,GL_STATIC_DRAW);
    glBindVertexArray(0);
    return true;
}
bool render(GLSLProgram &proc,unsigned int &vaoHandle,unsigned int bufferHandle[3]){
    bool success=compile(proc);
    if(!success){
        return false;
    }
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t>shapes;
    std::vector<tinyobj::material_t>materials;
    std::string err;
    const char *file="pig_triangulated.obj";
    success=tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file,nullptr);
    if(!success)
        return false;
    int totalCount=0,indexNow=0,totalVertices;
    for(int i=0;i<shapes.size();i++){
        for(int j=0;j<shapes[i].mesh.num_face_vertices.size();j++){
            totalCount+=shapes[i].mesh.num_face_vertices[j];
        }
    }
    totalVertices=attrib.vertices.size();
    float *vertices=new float[totalVertices];
    float *normal=new float[totalVertices];
    for(int i=0;i<totalVertices;i++){
        vertices[i]=attrib.vertices[i];
        normal[i]=0;
    }
    face=totalCount;
    unsigned int *element=new unsigned int[totalCount];
    for(int i=0;i<shapes.size();i++){
        size_t index_offset=0;
        for(int j=0;j<shapes[i].mesh.num_face_vertices.size();j++){
            int fv=shapes[i].mesh.num_face_vertices[j];
            element[indexNow*3+0]=shapes[i].mesh.indices[index_offset].vertex_index;
            element[indexNow*3+1]=shapes[i].mesh.indices[index_offset+1].vertex_index;
            element[indexNow*3+2]=shapes[i].mesh.indices[index_offset+2].vertex_index;
            glm::vec3 a=glm::vec3(vertices[element[indexNow*3+0]*3],vertices[element[indexNow*3+0]*3+1],vertices[element[indexNow*3+0]*3+2]);
            glm::vec3 b=glm::vec3(vertices[element[indexNow*3+1]*3],vertices[element[indexNow*3+1]*3+1],vertices[element[indexNow*3+1]*3+2]);
            glm::vec3 c=glm::vec3(vertices[element[indexNow*3+2]*3],vertices[element[indexNow*3+2]*3+1],vertices[element[indexNow*3+2]*3+2]);
            glm::vec3 ab=b-a;
            glm::vec3 ac=c-a;
            glm::vec3 norm=glm::cross(ab, ac);
            norm=glm::normalize(norm);
            normal[element[indexNow*3+0]*3]+=norm.x;
            normal[element[indexNow*3+0]*3+1]+=norm.y;
            normal[element[indexNow*3+0]*3+2]+=norm.z;
            normal[element[indexNow*3+1]*3]+=norm.x;
            normal[element[indexNow*3+1]*3+1]+=norm.y;
            normal[element[indexNow*3+1]*3+2]+=norm.z;
            normal[element[indexNow*3+2]*3]+=norm.x;
            normal[element[indexNow*3+2]*3+1]+=norm.y;
            normal[element[indexNow*3+2]*3+2]+=norm.z;
            indexNow++;
            index_offset+=3;
        }
    }
    for(int i=0;i<totalVertices/3;i++){
        glm::vec3 normalTemp=glm::vec3(normal[i*3],normal[3*i+1],normal[3*i+2]);
        normalTemp=glm::normalize(normalTemp);
        normal[i*3]=normalTemp.x;
        normal[i*3+1]=normalTemp.y;
        normal[i*3+2]=normalTemp.z;
    }
    glGenBuffers(3,bufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER,bufferHandle[0]);
    glBufferData(GL_ARRAY_BUFFER,totalVertices*sizeof(float),vertices,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,bufferHandle[1]);
    glBufferData(GL_ARRAY_BUFFER,totalVertices*sizeof(float),normal,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,bufferHandle[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,totalCount*sizeof(unsigned int),element,GL_STATIC_DRAW);
    delete [] vertices;
    delete [] normal;
    delete [] element;
    glGenVertexArrays(1,&vaoHandle);
    glBindVertexArray(vaoHandle);
    glBindBuffer(GL_ARRAY_BUFFER,bufferHandle[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)nullptr);
    glBindBuffer(GL_ARRAY_BUFFER,bufferHandle[1]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,bufferHandle[2]);
    glBindVertexArray(0);
//    mat4 view = glm::lookAt(vec3(0.0f,3.0f,4.0f), vec3(-2.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    mat4 view = glm::lookAt(vec3(1.0f,3.0f,1.0f), vec3(-1.0f,1.0f,-1.0f), vec3(0.0f,1.0f,0.0f));
    mat4 projection = glm::perspective(70.0f, 1.0f, 0.03f, 100.0f);
    mat3 normalMatrix = mat3( vec3(view[0]), vec3(view[1]), vec3(view[2]) );
    char name[20];
    float x, z;
    for( int i = 1; i < 5; i++ ) {
        snprintf(name, 20, "Lights[%d].Position", i);
        x = 2.0 * cos((TWOPI / 5) * i);
        z = 2.0 * sin((TWOPI / 5) * i);
        proc.setUniform(name, view * vec4(x, 1.2f, z + 1.0f, 1.0f) );
    }
    mat4 model = mat4(1.0f);
    proc.setUniform("Lights[0].Position", view*vec4(0.0f,5.0f,0.0f,1.0f));
    vec3 light1pos=vec3(0.0f,-1.0f,0.0f);
    proc.setUniform("Lights[0].direction", normalMatrix*(light1pos));
    proc.setUniform("Lights[0].Intensity", vec3(0.9f,0.9f,0.9f) );
    proc.setUniform("Lights[1].Intensity", vec3(0.0f,0.0f,0.8f) );
    proc.setUniform("Lights[2].Intensity", vec3(0.8f,0.0f,0.0f) );
    proc.setUniform("Lights[3].Intensity", vec3(0.0f,0.8f,0.0f) );
    proc.setUniform("Lights[4].Intensity", vec3(0.8f,0.8f,0.8f) );
    proc.setUniform("Lights[0].enableSpotLight", true);
    proc.setUniform("Lights[0].exponent", 2.0f);
    proc.setUniform("Lights[0].cutoff", 0.15f*3.1415f);
    
    proc.setUniform("Lights[1].enableSpotLight", false);
    proc.setUniform("Lights[2].enableSpotLight", false);
    proc.setUniform("Lights[3].enableSpotLight", false);
    proc.setUniform("Lights[4].enableSpotLight", false);
//    proc.setUniform("Lights[1].Intensity", vec3(0.0f,0.0f,0.0f) );
//    proc.setUniform("Lights[2].Intensity", vec3(0.0f,0.0f,0.0f) );
//    proc.setUniform("Lights[3].Intensity", vec3(0.0f,0.0f,0.0f) );
//    proc.setUniform("Lights[4].Intensity", vec3(0.0f,0.0f,0.0f) );
    proc.setUniform("Kd", 0.4f, 0.4f, 0.4f);
    proc.setUniform("Ks", 0.9f, 0.9f, 0.9f);
    proc.setUniform("Ka", 0.1f, 0.1f, 0.1f);
    proc.setUniform("Shininess", 180.0f);
    
    //model = glm::rotate(model,90.0f, vec3(0.0f,1.0f,0.0f));
    mat4 mv = view * model;
    proc.setUniform("ModelViewMatrix", mv);
    
    proc.setUniform("NormalMatrix",mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    proc.setUniform("MVP", projection*mv );
    glEnable(GL_DEPTH_TEST);
    return true;
}
bool compile(GLSLProgram &proc){
    bool success=false;
    success=proc.compileShaderFromFile("improved.vert", GLSLShader::VERTEX);
    if(!success){
        std::cout<<proc.log()<<std::endl;
        return false;
    }
    success=proc.compileShaderFromFile("improved.frag", GLSLShader::FRAGMENT);
    if(!success){
        std::cout<<proc.log()<<std::endl;
        return false;
    }
    if(!proc.link()){
        std::cout<<"program is not link"<<std::endl;
        std::cout<<proc.log()<<std::endl;
        return false;
    }
    proc.use();
    return true;
}
int init(){
    if(!glfwInit()){
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // Create a GLFWwindow object that we can use for GLFW's functions
    window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
   // glViewport(0, 0, WIDTH, HEIGHT);
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    GLenum err=glewInit();
    if(GLEW_OK!=err){
        std::cout<<"Error to initializing GLEW: "<<glewGetString(err)<<std::endl;
        return -1;
    }
    return 0;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(key==GLFW_KEY_W&&action==GLFW_PRESS){
        glUseProgram(handle);
        if(change){
            GLuint diffuseIndex=glGetSubroutineIndex(handle,GL_VERTEX_SHADER,"diffuseOnly");
            glUniformSubroutinesuiv(GL_VERTEX_SHADER,1,&diffuseIndex);
            change=false;
        }else{
            GLuint adsIndex=glGetSubroutineIndex(handle,GL_VERTEX_SHADER,"phoneModel");
            glUniformSubroutinesuiv(GL_VERTEX_SHADER,1,&adsIndex);
            change=true;
        }
    }
}
