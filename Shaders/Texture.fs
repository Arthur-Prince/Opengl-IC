#version 460 core

uniform sampler2D Texture;

in vec4 mouse;
in vec2 R;
in float time;
in vec2 posV;

out vec4 fragColor;

void main(){
    vec4 pixel = texture(Texture, posV);
    fragColor=pixel;

}