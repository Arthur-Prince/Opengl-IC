#version 460 core

uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform sampler2D Texture3;
uniform sampler2D bo;

in vec4 mouse;
in vec2 R;
in float time;
in vec2 posV;

out vec4 fragColor;

#include <common.fs>

void main(){
    vec2 pos = gl_FragCoord.xy;
    ivec2 p = ivec2(pos);
    //vec4 pixel = texture(Texture1,posV);
    vec4 b = texture(bo,pos/R);
    // if(pixel.w<-1.0){
    //     pixel.w=0.;
    // }
    vec4 rtn;// = (pixel)*pixel.w;
    if(b.x ==0. && b.y ==0. && b.z ==0.){
        rtn = vec4(1.);
    }
    fragColor=rtn;

}