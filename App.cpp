#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "stb_image/stb_image.h"

#include <fstream>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <unistd.h>
#include <filesystem>

//#include "VertexArray.h"
#include "Vertex.h"

#include "VertexObj.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window,double *mouse, bool * shouldDraw,  bool * press);

static int erros = 0;

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;


static float vertices[32] = {
    1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};

static unsigned int indices[6] = {
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


int main(){

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




    ////////////////shader/////////////

    std::string parentPath = (path.parent_path()).c_str();

    const std::string vertexShader = "/Shaders/draw.vs";
    const std::string vertexPath = parentPath + vertexShader;
    const char * vertexShaderPath = vertexPath.c_str();


    const std::string fragmentShader = "/Shaders/draw.fs";
    const std::string fragmentPath = parentPath + fragmentShader;
    const char * fragmentShaderPath = fragmentPath.c_str();

    const std::string fragmentShader2 = "/Shaders/view.fs";
    const std::string fragmentPath2 = parentPath + fragmentShader2;
    const char * fragmentShaderPath2 = fragmentPath2.c_str();

    Shader Shaderprogram(vertexShaderPath,fragmentShaderPath);

    Shader view(vertexShaderPath,fragmentShaderPath2);


    ///////////vertex/////////////


    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),(void *)0 );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),(void *) (3*sizeof(float)) );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float),(void *) (6*sizeof(float)) );
    glEnableVertexAttribArray(2);



    // VertexArray va;
    // va.bind();

    
    // VertexBuffer vb = VertexBuffer(vertices, 4*8*sizeof(float));

    // VertexBufferElement ib = VertexBufferElement(indices, 6);
    // ib.bind();
    
    // VertexBufferLayout layout;

    
    // layout.pushF(3);
    // layout.pushF(3);
    // layout.pushF(2);
 

    // va.addBuffer(vb, layout);



    ////////////////////
    //  FRAMEBUFFER   //
    ////////////////////

     

    //Criando o framebuffer
    unsigned int FBO;
    glGenFramebuffers(1, &FBO);

    //Ligando o framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER,FBO);     

    //Para utilizar o framebuffer, é necessário ligar a ele uma textura
    unsigned int textura0;

    glGenTextures(1, &textura0);
    glBindTexture(GL_TEXTURE_2D, textura0);
     
    //Gerando uma textura vazia para ligar ao framebuffer
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // glGenerateMipmap(GL_TEXTURE_2D);

    unsigned int textura1;

    glGenTextures(1, &textura1);
    glBindTexture(GL_TEXTURE_2D, textura1);
     
    //Gerando uma textura vazia para ligar ao framebuffer
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    //LIGANDO A TEXTURA AO FRAMBUFFER

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textura0, 0 );
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, textura1, 0 );
    GLenum attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);








    // unsigned int PBO, PBO2;

    // glGenBuffers(1, &PBO);
    // glBindBuffer(GL_PIXEL_UNPACK_BUFFER,PBO);
    // glBufferData(GL_PIXEL_UNPACK_BUFFER, 800*600, 0, GL_STREAM_DRAW);

    // // map the buffer object into client's memory
    // GLubyte* ptr = (GLubyte*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_READ_ONLY);
    // if(ptr)
    // {
    //     std::cout << ptr << std::endl;
    //     // update data directly on the mapped buffer
    //     glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER); // release the mapped buffer
    // }

    // // it is good idea to release PBOs with ID 0 after use.
    // // Once bound with 0, all pixel operations are back to normal ways.
    // glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    








    unsigned int text[2] = {textura0,textura1};






    ////////////init/////////////


    glm::vec2 resolution(SCR_WIDTH,SCR_HEIGHT);

    Shaderprogram.use();
    Shaderprogram.setVec2("iResolution",resolution);

    double * mouse = (double *) malloc(sizeof(double)*4);
    bool shouldDraw = true;
    bool press = false;
    int textNumber = 0;
    struct timeval ant{};
    struct timeval dps{};
    gettimeofday(&ant, nullptr);
    gettimeofday(&dps, nullptr);
    int periodo = 0;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, text[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, text[1]);

    /////////////////loop///////////


    while(!glfwWindowShouldClose(window)){

        processInput(window,mouse, &shouldDraw, & press);

        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        ant = dps;
        gettimeofday(&dps, nullptr);
        //periodo em ms
        periodo = int(((dps.tv_sec * 1000) + (dps.tv_usec / 1000)) - ((ant.tv_sec * 1000) + (ant.tv_usec / 1000)));

        Shaderprogram.use();
        Shaderprogram.setInt("Periodo", periodo);
        Shaderprogram.setSampler("Texture",0);
        Shaderprogram.setSampler("Texture2",1);
        Shaderprogram.setVec2("iResolution",resolution);
        Shaderprogram.setVec4("mouse", mouse);
        //std::cout << periodo<< std::endl;
        

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,nullptr);
        
        //sleep(1);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


        view.use();
        glActiveTexture(GL_TEXTURE0+ 0);
        glBindTexture(GL_TEXTURE_2D, text[0]);
        view.setSampler("Texture",0);
        //textNumber++;
        glActiveTexture(GL_TEXTURE0+ 1);
        glBindTexture(GL_TEXTURE_2D, text[1]);
        view.setSampler("Texture2",1);
        view.setVec2("iResolution",resolution);
        view.setVec4("mouse", mouse);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,nullptr);


        glfwSwapBuffers(window);
        glfwPollEvents();


    }

    // q.~QuadFullScr();

    // va.~VertexArray();
    // vb.~VertexBuffer();
    // ib.~VertexBufferElement();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glDeleteFramebuffers(1, &FBO);

    glfwTerminate();
    return 0;

}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
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