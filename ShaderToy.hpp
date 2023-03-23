#include "OpenGLAPI.hpp"
#include "Shader.h"

namespace ShaderToy{

    

    class Buffers{

        private:
        char Me;
        bool autoreferencia = true;

        public:

        Buffers(char Me, unsigned int Height,unsigned int Width){
            this->Me = Me;


        }


    };

    class BufferA: public Buffers{

    };

}