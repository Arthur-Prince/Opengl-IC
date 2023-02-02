#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "stb_image/stb_image.h"

#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <filesystem>

#include "VertexArray.h"
#include "Vertex.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window,double *mouse, bool * shouldDraw,  bool * press);

static int erros = 0;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float vertices[] = {
    1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};

unsigned int indices[] = {
    0,1,3,
    1,2,3
};

static void getError(){
    erros++;
    std::cout << erros << std::endl;
    while(GLenum error = glGetError()){
        std::cout << std::hex << "openGL: " << error << std::endl;
    }
}

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

    std::string parentPath = (path.parent_path()).c_str();

    const std::string vertexShader = "/Shaders/VertexShader.vs";
    const std::string vertexPath = parentPath + vertexShader;
    const char * vertexShaderPath = vertexPath.c_str();


    const std::string fragmentShader = "/Shaders/FragmentShader.fs";
    const std::string fragmentPath = parentPath + fragmentShader;
    const char * fragmentShaderPath = fragmentPath.c_str();
   
    /*const std::string fragmentShader2 = "/Shaders/FragmentShader2.fs";
    const std::string fragmentPath2 = parentPath + fragmentShader2;
    const char * fragmentShader2Path = fragmentPath2.c_str();*/

    const std::string fragmentShader3 = "/Shaders/FragmentShader3.fs";
    const std::string fragmentPath3 = parentPath + fragmentShader3;
    const char * fragmentShader3Path = fragmentPath3.c_str();

    Shader Shaderprogram(vertexShaderPath,fragmentShaderPath);
    //Shader Shaderprogram2(vertexShaderPath,fragmentShader2Path);
    Shader Shaderprogram3(vertexShaderPath,fragmentShader3Path);

    ////////////////////
    // OPENGL BÁSICOS //
    ////////////////////
    // unsigned int VBO, VAO, EBO;
    // glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);
    // glBindVertexArray(VAO);

    // glBindBuffer(GL_ARRAY_BUFFER,VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices, GL_STREAM_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),(void *)0 );
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),(void *) (3*sizeof(float)) );
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float),(void *) (6*sizeof(float)) );
    // glEnableVertexAttribArray(2);


    
    VertexArray va;
    va.bind();
    
    VertexBuffer vb(vertices, 8*4*sizeof(float));
    

    VertexBufferElement ib(indices, 6);
    ib.bind();
    
    VertexBufferLayout layout;

    
    layout.pushF(3);
    layout.pushF(3);
    layout.pushF(2);

    va.addBuffer(vb, layout);

    
    

    ////////////////////
    //    TEXTURES    //
    ////////////////////

    //Abrindo textura presente nas pastas do projeto]
    //const std::string container = "/../Mielke.png";
    //const std::string texturePath = pathString + container;

    //std::cout << texturePath << std::endl;
    //const char * finalPath = texturePath.c_str();
    //std::cout << "finalPath:" << finalPath << std::endl;
    //int width, height, nrChannels;
    //unsigned char *data = stbi_load(finalPath, &width, &height, &nrChannels, 0);

    //Gerando o objeto opengl de textura
    unsigned int textura;
    glGenTextures(1,&textura);
    glBindTexture(GL_TEXTURE_2D, textura);

    //Definindo como a textura faz wrapping (como ela se comporta nas bordas)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Definindo como a textura se comporta quando escalada para resoluções maiores ou menores
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Colocando a imagem lida no objeto opengl de textura
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glGenerateMipmap(GL_TEXTURE_2D);

    //stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);


    //printf("hello");

    ////////////////////
    //  FRAMEBUFFER   //
    ////////////////////

     

    //Criando o framebuffer
    unsigned int FBO;
    glGenFramebuffers(1, &FBO);

    //Ligando o framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER,FBO);     

    //Para utilizar o framebuffer, é necessário ligar a ele uma textura
    unsigned int texturaFBO;

    glGenTextures(1, &texturaFBO);
    glBindTexture(GL_TEXTURE_2D, texturaFBO);
     
    //Gerando uma textura vazia para ligar ao framebuffer
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    //LIGANDO A TEXTURA AO FRAMBUFFER

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texturaFBO, 0 );

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    double * mouse = (double *) malloc(sizeof(double)*4);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textura); 
    //Shaderprogram2.use();
    //Shaderprogram2.setSampler("Texture");
    glm::vec2 resolution(SCR_WIDTH,SCR_HEIGHT);
    //Shaderprogram2.setVec2("iResolution",resolution);
    Shaderprogram.use();
    Shaderprogram.setVec2("iResolution",resolution);

    bool shouldDraw = true;
    bool press = false;
    ////////////////////
    //  RENDER LOOP   //
    ////////////////////
    while(!glfwWindowShouldClose(window)){
        
        //Renderizando para o framebuffer criado
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
         processInput(window,mouse, &shouldDraw, & press);

        //Renderizando para o buffer padrão

        //Utilização de um segundo shader para calcular vizinhança
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texturaFBO); 
        Shaderprogram3.use();
        Shaderprogram3.setSampler("Texture");
        Shaderprogram3.setVec2("iResolution",resolution);
        Shaderprogram3.setVec4("mouse", mouse);

        if(shouldDraw) glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

        //Renderizando para a tela
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texturaFBO);

        Shaderprogram.use();
        Shaderprogram.setSampler("Texture");
        Shaderprogram.setVec2("iResolution",resolution);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);


        glfwSwapBuffers(window);
        glfwPollEvents();


    }

    va.~VertexArray();
    vb.~VertexBuffer();
    ib.~VertexBufferElement();

    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);
    glDeleteFramebuffers(1, &FBO);

    glfwTerminate();
    return 0;
}

 void processInput(GLFWwindow *window,double *mouse, bool * shouldDraw, bool * press)
{
    //Caso aperte 'esc', indica que a janela deve ser fechada
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    //Alocando o vetor que contém o mouse
    //double * mouse = (double *) malloc(sizeof(double)*4);

    //Encontrando a posição do mouse
    glfwGetCursorPos(window,&mouse[0], &mouse[1]);
    mouse[2] = 0.0;
    mouse[3] = 0.0;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) mouse[2] = 1.0; 
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) mouse[3] = 1.0;

    //Normalizando as coordenadas do cursor
    int h,w;
    glfwGetFramebufferSize(window, &w, &h);
    //mouse[1] = h-mouse[1];
    mouse[0] = ((mouse[0]*2.0)/w) -1.0;
    mouse[1] = ((mouse[1]*2.0)/h) -1.0;
    //return mouse;

    if(glfwGetKey(window, 32) == GLFW_PRESS && !*press) {
        *shouldDraw = !*shouldDraw;
        * press = true;
    }
    if(glfwGetKey(window, 32) == GLFW_RELEASE) * press = false;
}

//call: chamada quando um determinado evento acontece
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    printf("%i:%i\n", width,height);
}
