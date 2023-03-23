#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "OpenGLAPI.hpp"
#include "Shader.h"

#include <vector>

class Framebuffer
    {
        private:
        unsigned int FBO;
        std::vector<OpenGLAPI::Texture> textures;
        unsigned int numOutput = 0;
        Shader *shader = nullptr;

        public:

        Framebuffer(){
            glGenFramebuffers(1, &FBO);

            glBindFramebuffer(GL_FRAMEBUFFER,FBO); 
        }

        ~Framebuffer(){
            glDeleteFramebuffers(1, &FBO);
        }

        void bind(){
            glBindFramebuffer(GL_FRAMEBUFFER,FBO);
        }

        
        void addOutputTexture(unsigned int Height,unsigned int Width, unsigned int numOutput =1){
            //bind();
            int ant = this->numOutput;
            this->numOutput += numOutput;

            for(int i = 0; i<numOutput;i++){

                textures.push_back(OpenGLAPI::Texture(Height,Width));

                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+ ant +i, GL_TEXTURE_2D, textures[ant+ i], 0 );


                if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "ERROR::FRAMEBUFFER:: Framebuffer A is not complete!" << std::endl;
            }
        }

        void addTexture(OpenGLAPI::Texture text){
            //bind();
            textures.push_back(text);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, text, 0);
            this->numOutput++;
        }



        
        void saveImage(){
            //bind();
            for(int i = 0; i<numOutput;i++){
                glActiveTexture(GL_TEXTURE0 + i);
                textures[i].bind();
            }
        }

        //numero do output layout
        void getTexture(unsigned int outputNumber){
            //bind();
            glActiveTexture(GL_TEXTURE0 + outputNumber);
            textures[outputNumber].bind();
        }

        //numero do output layout e o numero da textura
        void getTexture(unsigned int textNumber, unsigned int outputNumber){
            //bind();
            glActiveTexture(GL_TEXTURE0 + textNumber);
            textures[outputNumber].bind();
        }

        Shader getShader(){
            //bind();
            return *shader;
        }

        void setshader(Shader *s){
            //bind();
            this->shader = s;

        }

    }; 