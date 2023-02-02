#pragma once

#include <glad/glad.h>
#include <vector>


struct vertexBufferElements
    {
        unsigned int type;
        unsigned int count;
        unsigned char normalize;


        static unsigned long getSizeOfType(unsigned int type){
            switch (type)
            {
            case GL_FLOAT:          return 4;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_BYTE:  return 1;
            }
            return 0;

        }
    };

class VertexBufferLayout{
    

    private:
        std::vector<vertexBufferElements> m_elements;
        unsigned int m_stride;

    public:

    VertexBufferLayout(): m_stride(0){};

    void pushF(unsigned int count){
        m_elements.push_back({GL_FLOAT, count, GL_FALSE});
        m_stride += count * vertexBufferElements::getSizeOfType(GL_FLOAT);

    }

    void pushInt(unsigned int count){
        m_elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
        m_stride += count * vertexBufferElements::getSizeOfType(GL_UNSIGNED_INT);
    }

    void pushChar(unsigned int count){
        m_elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
        m_stride += count * vertexBufferElements::getSizeOfType(GL_UNSIGNED_BYTE);
    }

    inline std::vector<vertexBufferElements> getElements(){
        return m_elements;
    }

    inline unsigned int getStride(){
        return m_stride;
    }

};




class VertexBuffer{

    private:
        unsigned int VBO;
    
    public:

    VertexBuffer(const void* data, unsigned int size){
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW);
    }

    ~VertexBuffer(){
        glDeleteBuffers(1, &VBO);
    }

    void bind() {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    }

    void unbind(){
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


};




class VertexBufferElement{

    private:
        unsigned int EBO;
        unsigned int m_count;
    
    public:

    VertexBufferElement(const unsigned int* data, unsigned int count): m_count(count)
    {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count* sizeof(unsigned int), data, GL_STREAM_DRAW);
    }

    ~VertexBufferElement(){
        glDeleteBuffers(1, &EBO);
    }

    void bind(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    }

    void unbind(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    inline unsigned int getCount(){return m_count;}

};


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