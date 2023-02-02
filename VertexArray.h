#pragma once


#include <glad/glad.h>
#include "Vertex.h"


class VertexArray{
    private:
        unsigned int m_renderID;

    int erros = 0;

    void getError(){
    erros++;
    std::cout << erros << std::endl;
    while(GLenum error = glGetError()){
        std::cout << std::hex << "openGL: " << error << std::endl;
    }
}

    public:


    VertexArray(){
        glGenVertexArrays(1, &m_renderID);
    }

    ~VertexArray(){
        glad_glDeleteVertexArrays(1, &m_renderID);

    }

    void addBuffer(VertexBuffer vb, VertexBufferLayout layout){

        bind();
        //vb.bind();

        unsigned long offset = 0;
    
        const auto& elements = layout.getElements();
        for (int i = 0; i < elements.size(); i++)
        {
            const auto& element = elements[i];
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, element.count, element.type, element.normalize,
            layout.getStride(),(void*) offset);
            

            offset += element.count * vertexBufferElements::getSizeOfType(element.type);

        }
        
    }

    void bind()const{
        glBindVertexArray(m_renderID);
    }

    void unbind()const{
        glBindVertexArray(0);
    }


};