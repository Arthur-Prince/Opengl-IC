#version 460 core

uniform sampler2D bufferB;
uniform sampler2D cor;
uniform int Frame;
uniform sampler2D stop;
in vec4 mouse;
in vec2 R;
in float time;
in vec2 posV;

out vec4 U;
out vec4 ColorMass;

#include <common.fs>

//diffusion and advection basically
void Reintegration(sampler2D ch, sampler2D ch2, inout particle P, vec2 pos)
{
    //basically integral over all updated neighbor distributions
    //that fall inside of this pixel
    //this makes the tracking conservative
    range(i, -2, 2) range(j, -2, 2)
    {
        vec2 tpos = pos + vec2(i,j);
        vec4 data = texel(ch, tpos);
        vec4 massa = texel(ch2, tpos);
       
        particle P0 = getParticle(data, massa, tpos);
       
        P0.X += P0.V*dt; //integrate position

        float difR = waterDiffusionRadius(P0.M.w);
        vec3 D = distribution(P0.X, pos, difR);
        //the deposited mass into this cell
        float m = P0.M.w*D.z;
        
        //add weighted by mass
        P.X += D.xy*m;
        P.V += P0.V*m;
        P.M.x += P0.M.x*m;
        P.M.y += P0.M.y*m;
        P.M.z += P0.M.z*m;
        
        //add mass
        P.M.w += m;
    }
    
    //normalization
    if(P.M.w != 0.)
    {
        P.X /= P.M.w;
        P.V /= P.M.w;
        P.M.x /= P.M.w;
        P.M.y /= P.M.w;
        P.M.z /= P.M.w;
    }
}

void main( )
{
    vec2 pos = gl_FragCoord.xy;

    vec4 info = texture(stop,posV);

    // ivec2 p = ivec2(pos);
    // vec4 data = texel(bufferB, pos); 
    // vec4 massa = texel(cor, pos); 

    particle P;// = getParticle(data, massa, pos);
    P.X = vec2(0.0);
    P.V = vec2(0.);
    P.M = vec4(0.);
    
    // Reintegration(bufferB, P, pos);
   
    // initial condition
    if(Frame < 1)
    {
        //random
        vec3 rand = hash32(pos); //hash32 = general kernel function
        if(rand.z < 0.0) 
        {
            
            P.X = pos;
            P.V = 0.5*(rand.xy-0.5) + vec2(0., -0.5);
            P.M = mix(P.M, vec4(1.0-rand.z,1.0-rand.z,1.0-rand.z, fluid_rho), 0.4);
        }
        else
        {
            P.X = pos;
            P.V = vec2(0.0);
            P.M = vec4(0.0);
            if(info.z>0. && (info.y!=0. && info.x==0.)){
                vec3 cor = vec3(0.,1.,0.);

                P.M = vec4(cor,info.b);
                P.V = -info.xy;
            }
            //P.M = vec4(texture(stop,posV).rgb,1.);
        }
    }


   
    Reintegration(bufferB, cor, P, pos);
    fBorda(stop,P,pos);
    

    // range(i, -1, 1) range(j, -1, 1){
    //     vec2 t = vec2(i,j);
    //     vec4 pixel = texture(stop, posV+t);
    //     if(pixel.x <0.1 && pixel.y <0.1 && pixel.z <0.1){


    //             P.V = vec2(0.,0.);
    //     }
    // }
    vec4[2] aux = saveParticle(P, pos);
    
    U = aux[0];
    ColorMass = aux[1];
    // U=vec4(1.0);
    // ColorMass = vec4(0.0);
}
