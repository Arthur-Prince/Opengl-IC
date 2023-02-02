#version 460 core

in vec3 cor;
in vec2 Coord;

out vec4 FragColor;

uniform sampler2D Texture;
uniform sampler2D Texture2;
uniform vec4 mouse;
uniform vec2 iResolution;

void main(){
    FragColor = vec4(texture(Texture2, (Coord)));
    //FragColor = vec4(0.0,1.0,0.0,1.0);
}

