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
    proc.bindAttribLocation(0, "VertexPosition");
    proc.bindAttribLocation(1, "VertexColor");
    proc.link();
    GLuint index=glGetAttribLocation(handle,"VertexPosition");
    std::cout<<index;
    index=glGetAttribLocation(handle,"VertexColor");
    std::cout<<index;
    
    GLfloat positionData[]={
        -0.8f, -0.8f, 0.0f,
         0.8f, -0.8f, 0.0f,
         0.0f,  0.8f, 0.0f
    };
    GLfloat colorData[]={
        1.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,
        0.0f,0.0f,1.0f
    };
    GLuint vaoHandle,vboHandle[2];
    glGenBuffers(2,vboHandle);
    glBindBuffer(GL_ARRAY_BUFFER,vboHandle[0]);
    glBufferData(GL_ARRAY_BUFFER,9*sizeof(GLfloat),positionData,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,vboHandle[1]);
    glBufferData(GL_ARRAY_BUFFER,9*sizeof(GLfloat),colorData,GL_STATIC_DRAW);
    glGenVertexArrays(1,&vaoHandle);
    glBindVertexArray(vaoHandle);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,vboHandle[0]);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)nullptr);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER,vboHandle[1]);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)nullptr);
    glBindVertexArray(0);
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        proc.use();
        glBindVertexArray(vaoHandle);
        glDrawArrays(GL_TRIANGLES, 0, 3);
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
    glViewport(0, 0,WIDTH  , HEIGHT);
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
