#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "OpenGLAPI.hpp"
#include "Shader.h"
#include "Framebuffer.h"
#include "stb_image/stb_image.h"


#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <filesystem>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


float vertices[] = {
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // top right
     1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f // top left 
};

unsigned int indices[] = {
    0,1,3,
    1,2,3
};

int main()
{
    
    if (__cplusplus == 201703L)
        std::cout << "C++17" << std::endl;
    else if (__cplusplus == 201402L)
        std::cout << "C++14" << std::endl;
    else if (__cplusplus == 201103L)
        std::cout << "C++11" << std::endl;
    else if (__cplusplus == 199711L)
        std::cout << "C++98" << std::endl;
    else
        std::cout << "pre-standard C++" << std::endl;

    std::filesystem::path path = std::filesystem::current_path();
    const std::string pathString = path.string();

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Textura", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glfwSwapInterval(0);
    //Habilita mensagens de debug

    OpenGLAPI::DebugManager* debugManager = nullptr;
    debugManager = OpenGLAPI::DebugManager::getDebugManager();
    //debugManager->DisableDebug();

    std::string parentPath = (path.parent_path()).c_str();

    const std::string vertexShader = "/Shaders/VertexShader.vs";
    const std::string vertexPath = parentPath + vertexShader;
    const char * vertexShaderPath = vertexPath.c_str();

    const std::string A = "/Shaders/BufferA.fs";
    const std::string APath = parentPath + A;
    const char * AShaderPath = APath.c_str();

    const std::string B = "/Shaders/BufferB.fs";
    const std::string BPath = parentPath + B;
    const char * BShaderPath = BPath.c_str();

    const std::string C = "/Shaders/BufferC.fs";
    const std::string CPath = parentPath + C;
    const char * CShaderPath = CPath.c_str();

    const std::string D = "/Shaders/BufferD.fs";
    const std::string DPath = parentPath + D;
    const char * DShaderPath = DPath.c_str();

    const std::string Image = "/Shaders/image.fs";
    const std::string IPath = parentPath + Image;
    const char * IShaderPath = IPath.c_str();

    const std::string T = "/Shaders/Texture.fs";
    const std::string TPath = parentPath + T;
    const char * TShaderPath = TPath.c_str();

    Shader ShaderprogramA(vertexShaderPath,AShaderPath);
    Shader ShaderprogramB(vertexShaderPath,BShaderPath);
    Shader ShaderprogramC(vertexShaderPath,CShaderPath);
    //Shader ShaderprogramD(vertexShaderPath,DShaderPath);
    Shader ShaderProgramImage(vertexShaderPath,IShaderPath);
    Shader ShaderProgramText(vertexShaderPath,TShaderPath);


    ////////////////////
    // OPENGL BÁSICOS //
    ////////////////////
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float),(void *)0 );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float),(void *) (3*sizeof(float)) );
    glEnableVertexAttribArray(1);

    // ///////////////////////////
    // //        BufferA        //
    // ///////////////////////////

    // OpenGLAPI::Texture bufferA(SCR_HEIGHT,SCR_WIDTH);

    // ///////////////////////////
    // //        BufferB        //
    // ///////////////////////////

    // OpenGLAPI::Texture bufferB(SCR_HEIGHT, SCR_WIDTH);

    // ///////////////////////////
    // //        Bufferc        //
    // ///////////////////////////

    // OpenGLAPI::Texture bufferC(SCR_HEIGHT, SCR_WIDTH);


    // ////////////////////
    // //  FRAMEBUFFER   //
    // ////////////////////
    
    // std::cout << " bufferA: " << (GLuint) bufferA << " bufferB: " << (GLuint) bufferB << " bufferC: " << (GLuint) bufferC << std::endl;

    // //Criando o framebuffer
    // unsigned int FBO;
    // glGenFramebuffers(1, &FBO);

    // //Ligando o framebuffer
    // glBindFramebuffer(GL_FRAMEBUFFER,FBO);     


    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferA, 0 );


    // if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "ERROR::FRAMEBUFFER:: Framebuffer A is not complete!" << std::endl;

    // ////////////////////
    // // FRAMEBUFFER B  //
    // ////////////////////

     

    // //Criando o framebuffer
    // unsigned int FBOB;
    // glGenFramebuffers(1, &FBOB);

    // //Ligando o framebuffer
    // glBindFramebuffer(GL_FRAMEBUFFER,FBOB);     

    // //glDrawBuffers(1, {GL_NONE});


    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferB, 0 );


    // if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "ERROR::FRAMEBUFFER:: Framebuffer B is not complete!" << std::endl;

    // ////////////////////
    // // FRAMEBUFFER C  //
    // ////////////////////

     

    // //Criando o framebuffer
    // unsigned int FBOC;
    // glGenFramebuffers(1, &FBOC);

    // //Ligando o framebuffer
    // glBindFramebuffer(GL_FRAMEBUFFER,FBOC);     


    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferC, 0 );


    // if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "ERROR::FRAMEBUFFER:: Framebuffer C is not complete!" << std::endl;

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char *data = stbi_load("../lab.png", &width, &height, &nrChannels, 4);
    

    OpenGLAPI::Texture imag(height, width, data, GL_RGBA32F, GL_RGBA);

    if (data)
    {
        std::cout << "imag carregada" << std::endl;
        glActiveTexture(GL_TEXTURE5);
        imag.bind();
    }
    stbi_image_free(data);


    Framebuffer test;
    test.addOutputTexture(SCR_HEIGHT,SCR_WIDTH);
    test.setshader(&ShaderProgramText);


    Framebuffer FBOA;
    FBOA.addOutputTexture(SCR_HEIGHT,SCR_WIDTH,2);
    FBOA.setshader(&ShaderprogramA);

    Framebuffer FBOB;
    FBOB.addOutputTexture(SCR_HEIGHT,SCR_WIDTH,2);
    FBOB.setshader(&ShaderprogramB);

    Framebuffer FBOC;
    FBOC.addOutputTexture(SCR_HEIGHT,SCR_WIDTH,2);
    FBOC.setshader(&ShaderprogramC);

    Framebuffer FBOD;
    FBOD.addOutputTexture(SCR_HEIGHT,SCR_WIDTH,2);
    //FBOD.setshader(&ShaderprogramD);


    OpenGLAPI::InputManager * inputManager = nullptr;
    inputManager = OpenGLAPI::InputManager::getInputManager();
  
    int frame = 0;
    Shader atualShader= FBOA.getShader();


    ////////////////////
    //  RENDER LOOP   //
    ////////////////////
    while(!glfwWindowShouldClose(window)){
        //Coletando a informação da textura atualmente ativa e copiando para um espaço de memória
        inputManager->processInput(window);
        void * arrayInput = inputManager->getInput();





        
        //Renderizando para o framebuffer criado
        FBOA.bind();
        atualShader = FBOA.getShader();

        //Utilização de um segundo shader para calcular vizinhança
        atualShader.use();
        atualShader.setInt("Frame", frame);
        FBOB.getTexture(1,0);
        atualShader.setSampler("bufferB",1);
        FBOB.getTexture(2,1);
        atualShader.setSampler("cor",2);
        atualShader.setDefaultParams(arrayInput);

        glActiveTexture(GL_TEXTURE3);
        imag.bind();
        atualShader.setSampler("stop",3);




        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

        ////////////////////////////////////////
        FBOB.bind();
        atualShader = FBOB.getShader();

        atualShader.use();
        FBOA.getTexture(0,0);
        atualShader.setSampler("bufferA",0);
        FBOA.getTexture(4,1);
        atualShader.setSampler("cor",4);

        atualShader.setDefaultParams(arrayInput);

        glActiveTexture(GL_TEXTURE7);
        imag.bind();
        atualShader.setSampler("stop",7);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

        //////////////////////////////////////////
        // FBOC.bind();
        // atualShader = FBOC.getShader();

        // atualShader.use();
        // FBOA.getTexture(0,0);
        // atualShader.setSampler("bufferA",0);
        // FBOA.getTexture(1,1);
        // atualShader.setSampler("cor",1);
        // atualShader.setDefaultParams(arrayInput);

        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

        //////////////////////////////////////////

        //Renderizando para a tela
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        // glClear(GL_COLOR_BUFFER_BIT);

        ShaderProgramImage.use();
        FBOA.getTexture(0,0);
        ShaderProgramImage.setSampler("bufferA",0);
        FBOA.getTexture(3,1);
        ShaderProgramImage.setSampler("cor",3);
        FBOB.getTexture(1,0);
        ShaderProgramImage.setSampler("bufferB",1);
        FBOC.getTexture(2,0);
        ShaderProgramImage.setSampler("bufferC",2);

        glActiveTexture(GL_TEXTURE5);
        imag.bind();
        ShaderProgramImage.setSampler("stop",5);

        ShaderProgramImage.setDefaultParams(arrayInput);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);



        // FBOA.bind();
        // Shader a = FBOA.getShader();

        // a.use();


        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

        //Renderizando para o framebuffer criado
        // FBOB.bind();
        // atualShader = FBOB.getShader();

        //Utilização de um segundo shader para calcular vizinhança
        // atualShader.use();
        // atualShader.setInt("Frame", frame);
        // FBOB.getTexture(1,0);
        // atualShader.setSampler("bufferB",1);
        // FBOB.getTexture(2,1);
        // atualShader.setSampler("cor",2);
        // atualShader.setDefaultParams(arrayInput);

        // glActiveTexture(GL_TEXTURE3);
        // imag.bind();
        // atualShader.setSampler("stop",3);




        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

        // glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        // glClear(GL_COLOR_BUFFER_BIT);

        // atualShader = test.getShader();
        // atualShader.use();

        // glActiveTexture(GL_TEXTURE5);
        // imag.bind();
        // ShaderProgramImage.setSampler("bo",5);

        // FBOA.getTexture(0,0);
        // atualShader.setSampler("Texture0", 0);
        // FBOA.getTexture(1,1);
        // atualShader.setSampler("Texture1", 1);
        // FBOB.getTexture(2,0);
        // atualShader.setSampler("Texture2", 2);
        // FBOB.getTexture(3,1);
        // atualShader.setSampler("Texture3", 3);


        // atualShader.setDefaultParams(arrayInput);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);





  
        glfwSwapBuffers(window);
        glfwPollEvents();
        frame++;
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    FBOA.~Framebuffer();
    FBOB.~Framebuffer();
    FBOC.~Framebuffer();
    FBOD.~Framebuffer();
    test.~Framebuffer();

    glfwTerminate();
    return 0;
}

//callback: chamada quando um determinado evento acontece
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    printf("%i:%i\n", width,height);
}