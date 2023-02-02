#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 Color;
layout (location = 2) in vec2 TexCoord;

out vec3 cor;
out vec2 Coord;


void main(){
    

    gl_Position = vec4(aPos.x,aPos.y, 0.0, 1.0);
    cor = Color;
    Coord = TexCoord;

}