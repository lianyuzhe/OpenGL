//
//  main.cpp
//  OpenGLlesson_1
//
//  Created by lianyuzhe on 2017/1/25.
//  Copyright © 2017年 lianyuzhe. All rights reserved.
//
#include <GLSLProgram.hpp>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLUT/GLUT.h>
#include "shape.hpp"
const GLuint WIDTH = 800, HEIGHT = 800;
GLFWwindow* window=nullptr;
int init();
bool compile(GLSLProgram &proc);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
int main(int argc, const char * argv[]) {
    init();
    GLSLProgram proc;
    bool success=compile(proc);
    if(!success){
        return false;
    }
    GLint handle=proc.getHandle();
    glm::mat4 model=glm::mat4(1.0f);
    model=glm::rotate(model, -35.0f, glm::vec3(1.0f,0.0f,0.0f));
    model=glm::rotate(model,-35.0f, glm::vec3(0.0f,1.0f,0.0f));
    glm::mat4 view=glm::lookAt(glm::vec3(0.0f,0.0f,5.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
    
    glm::mat4 projection=glm::perspective(70.0f, (float)1.0f, 0.3f, 100.0f);
    //glm::mat4 projection=glm::mat4(1.0f);
    float *vertex,*normal,*textCoords;
    unsigned int *element;
    int nVerts=30*31;
    int faces=30*30;
    // Verts
    vertex = new float[3 * nVerts];
    // Normals
    normal = new float[3 * nVerts];
    // Tex coords
    textCoords = new float[2 * nVerts];
    // Elements
    element = new unsigned int[6 * faces];
    generateTours(vertex, normal, textCoords, element, 0.7f, 0.3f, 30, 30);
//    for(int i=0;i<3*nVerts;i++){
//        std::cout<<vertex[i]<<std::endl;
//    }
    unsigned int bufferHandle[4];
    glGenBuffers(4,bufferHandle);
    
    glBindBuffer(GL_ARRAY_BUFFER,bufferHandle[0]);
    glBufferData(GL_ARRAY_BUFFER,(3*nVerts)*sizeof(GLfloat),vertex,GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER,bufferHandle[1]);
    glBufferData(GL_ARRAY_BUFFER,(3*nVerts)*sizeof(GLfloat),normal,GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER,bufferHandle[2]);
    glBufferData(GL_ARRAY_BUFFER,(2*nVerts)*sizeof(GLfloat),textCoords,GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,bufferHandle[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,6*faces*sizeof(unsigned int),element,GL_STATIC_DRAW);
    delete []vertex;
    delete []normal;
    delete []textCoords;
    delete []element;
    
    GLuint vaoHandle;
    glGenVertexArrays(1,&vaoHandle);
    glBindVertexArray(vaoHandle);
    
    glBindBuffer(GL_ARRAY_BUFFER,bufferHandle[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)nullptr);
    
    glBindBuffer(GL_ARRAY_BUFFER,bufferHandle[1]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)nullptr);
    
//    glBindBuffer(GL_ARRAY_BUFFER,bufferHandle[2]);
//    glEnableVertexAttribArray(2);
//    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,(GLubyte*)nullptr);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,bufferHandle[3]);
    glBindVertexArray(0);
    
    proc.setUniform("Kd", 0.9f, 0.5f, 0.3f);
    proc.setUniform("Ld", 1.0f, 1.0f, 1.0f);
    proc.setUniform("LightPosition", view * glm::vec4(5.0f,5.0f,2.0f,1.0f) );
    glm::mat4 mv = view*model;
    proc.setUniform("ModelViewMatrix", mv);
    proc.setUniform("NormalMatrix",
                    glm::mat3( glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2]) ));
    proc.printActiveUniforms();
    proc.setUniform("MVP",projection*view*model);
    proc.use();
    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClearColor(0.2f,0.2f,0.2f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        const GLfloat MVP[]={1.0f,0.0f,0.0f,0.0f,
            0.0f,1.0f,0.0f,0.0f,
            0.0f,0.0f,1.0f,0.0f,
            0.0f,0.0f,0.0f,1.0f};
        glBindVertexArray(vaoHandle);
        glDrawElements(GL_TRIANGLES, 6 * faces, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
        glfwSwapBuffers(window);
        glBindVertexArray(0);
    }
    return 0;
}
bool compile(GLSLProgram &proc){
    bool success=false;
    success=proc.compileShaderFromFile("basic.vert", GLSLShader::VERTEX);
    if(!success){
        std::cout<<proc.log()<<std::endl;
        return false;
    }
    success=proc.compileShaderFromFile("basic.frag", GLSLShader::FRAGMENT);
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
}
