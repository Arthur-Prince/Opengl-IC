#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

void MessageCallback( GLenum source, GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar* message,const void* userParam );

namespace OpenGLAPI{


    //Classe responsável por manejar input/informações externas ao programa
    class InputManager{
        public:
        static InputManager * getInputManager()
        {
         static InputManager * manager = nullptr;
         if (!manager) manager = new InputManager;
         return manager;
        }

        void processInput(GLFWwindow *window){
            time = (float) glfwGetTime();
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
            getMouseValue(window);
            getKeysPressed(window);
            getInputArray();
            //printArray();
        }

        void * getInput(){
            return array;
        }

        float getTime(){
            return time;
        }

        double* getMouse(){
            return mouse;
        }

        private:

        double* mouse;
        bool shouldDraw;
        bool press;
        float time;
        int height;
        int width;


        void* array;

        InputManager()
        {
            mouse = (double*) malloc(sizeof(double)*4);
            shouldDraw = true;
            press = false;
            float time = (float) glfwGetTime();
        }

        void getMouseValue(GLFWwindow *window){
            glfwGetCursorPos(window,&mouse[0], &mouse[1]);
            mouse[2] = 0.0;
            mouse[3] = 0.0;
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) mouse[2] = 1.0; 
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) mouse[3] = 1.0;

            //Normalizando as coordenadas do cursor
            glfwGetFramebufferSize(window, &width, &height);
            mouse[0] = ((mouse[0]*2.0)/width) -1.0;
            mouse[1] = ((mouse[1]*2.0)/height) -1.0;
        }

        void getKeysPressed(GLFWwindow *window){ 
            if(glfwGetKey(window, 32) == GLFW_PRESS && !press) {
                shouldDraw = !shouldDraw;
                press = true;
            }
            if(glfwGetKey(window, 32) == GLFW_RELEASE) press = false;
        }

        void getInputArray(){
            void * array = malloc(sizeof(double*)*6 + sizeof(bool)*2 + sizeof(float));
            *(bool*) array = shouldDraw;
            *(bool*) (array + sizeof(bool)) = press;
            for(int i = 0; i < 4; i++) *(double *) (array + sizeof(double)*i + 2*sizeof(bool)) = mouse[i];
            *(double *) (array + sizeof(double)*4 + 2*sizeof(bool)) = (double) height;
            *(double *) (array + sizeof(double)*5 + 2*sizeof(bool)) = (double) width;
            *(float*) (array + sizeof(double)*6 + sizeof(bool)*2) = time;  
            this->array = (void*) array;
        }
    };


    //Classe responsável por ativar e desativar mensagens de debug

    class DebugManager
    {
        public:

        static DebugManager * getDebugManager(){
            static DebugManager * manager = nullptr;
            if(!manager) manager = new DebugManager;
            return manager;
        }

        void DisableDebug(){
            glDisable(GL_DEBUG_OUTPUT);
        }

        void EnableDebug(){
            glEnable(GL_DEBUG_OUTPUT);
        }

        private:

        DebugManager(){
        glEnable(GL_DEBUG_OUTPUT );
        glDebugMessageCallback(MessageCallback, 0 );
        }
    };


    //Classe para texturas
    class Texture
    {
        public:
        Texture(unsigned int Height,unsigned int Width, unsigned char* data = NULL, GLenum internalFormat = GL_RGBA32F,
            GLenum format = GL_RGBA, GLenum WrapParams_S = GL_REPEAT, GLenum WrapParams_T = GL_REPEAT,
            GLenum MagParams = GL_LINEAR, GLenum MinParams = GL_LINEAR)
        {
            glGenTextures(1,&textureID);
            glBindTexture(GL_TEXTURE_2D,textureID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapParams_S);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapParams_T);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinParams);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagParams);

            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, Width, Height, 0, format, GL_UNSIGNED_BYTE, data);

            this->Height = Height;
            this->Width = Width;

            glBindTexture(GL_TEXTURE_2D, 0);
        }
        
        operator GLuint(){
            return textureID;
        }

        void addTexture(unsigned char *data, int width, int height, int nrChannels){
            glBindTexture(GL_TEXTURE_2D,textureID);


            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                this->Height = height;
                this->Width = width;
            }
            else
            {
                std::cout << "Failed to load texture" << std::endl;
            }
        }

        

        void bind(){
            glBindTexture(GL_TEXTURE_2D, textureID);
        }

        unsigned int getID(){
            return textureID;
        }

        private:
        unsigned int textureID;
        unsigned int Height;
        unsigned int Width;
    };

    
} 




void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam )
        {
        fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ), type, severity, message );
        switch (source)
            {
                case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
                case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
                case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
                case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
                case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
            } std::cout << std::endl;
            switch (type)
            {
                case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
                case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
                case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
                case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
                case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
                case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
                case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
            } std::cout << std::endl;

            switch (severity)
            {
                case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
                case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
                case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
                case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
            } std::cout << std::endl;
            std::cout << std::endl;
        }