#pragma once

#include <glad/glad.h>

//#include "VertexArray.h"
#include "Vertex.h"
#include "Shader.h"


class QuadFullScr{
    private:
    
    VertexArray* VAO;
    VertexBufferElement* EBO;
    VertexBuffer* VBO;
    Shader* Shaderprogram;

    float vertices[32] = {
    1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

    unsigned int indices[6] = {
        0,1,3,
        1,2,3
    };



int erros = 0;

void getError(){
    erros++;
    std::cout << erros << std::endl;
    while(GLenum error = glGetError()){
        std::cout << std::hex << "openGL: " << error << std::endl;
    }
}





    public:

    QuadFullScr(Shader* sp )
    {

        

        Shaderprogram = sp;
        
        VAO = new VertexArray();
        VAO->bind();

        
        VBO = new VertexBuffer(vertices, sizeof(vertices));

        EBO = new VertexBufferElement(indices, 6);
        
        VertexBufferLayout layout;

        
        layout.pushF(3);
        layout.pushF(3);
        layout.pushF(2);
    

        VAO->addBuffer(*VBO, layout);

        
    }

    ~QuadFullScr(){
        VAO->~VertexArray();
        VBO->~VertexBuffer();
        EBO->~VertexBufferElement();
    }

    void test(){

        // VAO = new VertexArray();
        // VAO->bind();
        // unsigned int VBO, VAO2, EBO;
        // unsigned int VAO;
        // glGenVertexArrays(1, &VAO);

        // glGenBuffers(1, &VBO);
        // glGenBuffers(1, &EBO);

        //glBindVertexArray(VAO);


        // VBO2 = new VertexBuffer(vertices, sizeof(vertices));

        // EBO = new VertexBufferElement(indices, 6);
        // EBO->bind();
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

        // VertexBufferLayout layout;

        
        // layout.pushF(3);
        // layout.pushF(3);
        // layout.pushF(2);
        // getError();
    

        // VAO->addBuffer(*VBO2, layout);
        // getError();
    }

    void setup(double * mouse, glm::vec2 resolution){
        VAO->bind();
        EBO->bind();
        VBO->bind();

        //Shaderprogram->setVec2("iResolution",resolution);
        //Shaderprogram->setVec4("mouse", mouse);
    }


    void draw(){
        Shaderprogram->use();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,nullptr);

    }


};