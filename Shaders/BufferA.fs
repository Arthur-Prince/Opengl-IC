#version 460 core

uniform sampler2D bufferB;
uniform int Frame;
uniform sampler2D stop;
in vec4 mouse;
in vec2 R;
in float time;
in vec2 posV;

out vec4 U;

#include <common.fs>

void main( )
{
    vec2 pos = gl_FragCoord.xy;
    ivec2 p = ivec2(pos);
    vec4 data = texel(bufferB, pos); 

    particle P;// = getParticle(data, pos);
    P.X = vec2(0.);
    P.V = vec2(0.);
    P.M = vec2(0.);
    
    //Reintegration(bufferB, P, pos);
   
    //initial condition
    if(Frame < 1)
    {
        //random
        vec3 rand = hash32(pos); //hash32 = general kernel function
        if(rand.z < 0.) 
        {
            
            P.X = pos;
            P.V = 0.5*(rand.xy-0.5) + vec2(0., -0.5);
            P.M = mix(P.M, vec2(fluid_rho, 1.0-rand.z), 0.4);
        }
        else
        {
            P.X = pos;
            P.V = vec2(0.0);
            P.M = vec2(0.);
        }
    }
    Reintegration(bufferB, P, pos);

    range(i, -1, 1) range(j, -1, 1){
        vec2 t = vec2(i,j);
        vec4 pixel = texture(stop, posV+t);
        if(pixel.x <0.1 && pixel.y <0.1 && pixel.z <0.1){


                P.V = vec2(0.,0.);
        }
    }
    
    U = saveParticle(P, pos);
}
