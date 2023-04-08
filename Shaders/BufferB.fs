#version 460 core

uniform sampler2D bufferA;
uniform sampler2D cor;
uniform sampler2D stop;
in vec4 mouse;
in vec2 R;
in float time;
in vec2 posV;

out vec4 U;
out vec4 ColorMass;

#include <common.fs>

//force calculation and integration
void Simulation(sampler2D ch, sampler2D ch2, inout particle P, vec2 pos)
{
    //Compute the SPH force
    vec2 F = vec2(0.);
    vec3 avgV = vec3(0.);
    range(i, -2, 2) range(j, -2, 2)
    {
        vec2 tpos = pos + vec2(i,j);
        vec4 data = texel(ch, tpos);
        vec4 massa = texel(ch2, tpos);

        particle P0 = getParticle(data, massa, tpos);

        vec2 dx = P0.X - P.X;
        float avgP = 0.5*P0.M.w*(Pf(P.M) + Pf(P0.M)); 
        F -= 0.5*G(1.*dx)*avgP*dx;
        avgV += P0.M.w*G(1.*dx)*vec3(P0.V,1.);
    }
    avgV.xy /= avgV.z;

    //viscosity
    F += 0.*P.M.w*(avgV.xy - P.V);
    
    //gravity
    F += P.M.w*vec2(0., -0.0004);

    if(mouse.z > 0.)
    {
        vec2 dm =(mouse.xy - mouse.zw)/10.; 
        float d = distance(mouse.xy, P.X)/20.;
        F += 0.001*dm*exp(-d*d);
       // P.M.y += 0.1*exp(-40.*d*d);
    }
    
    //integrate
    P.V += F*dt/P.M.w;

    //border 
    vec3 N = bN(P.X);
    float vdotN = step(N.z, border_h)*dot(-N.xy, P.V);
    P.V += 0.5*(N.xy*vdotN + N.xy*abs(vdotN));
    P.V += 0.*P.M.w*N.xy*step(abs(N.z), border_h)*exp(-N.z);
    
    if(N.z < 0.) P.V = vec2(0.);




    
    
    //velocity limit
    float v = length(P.V);
    P.V /= (v > 1.)?v:1.;
    P.V = clamp(P.V,vec2(-1.0),vec2(1.0));
}


void main()
{

    vec2 pos = gl_FragCoord.xy;
    
    // ivec2 p = ivec2(pos);
        
    vec4 data = texture(bufferA, posV); 
    vec4 massa = texture(cor, posV);
    vec4 info = texture(stop, posV);
    
    
    
    particle P = getParticle(data, massa, pos);
    
    
    if(P.M.w != 0.) //not vacuum
    {
        Simulation(bufferA, cor, P, pos);
    }

    
    //vec2 proxP = posV+P.V;
    
    // if(length(P.X - R*vec2(0.1, 0.92)) < 20.)//se estiver dentro do alcance da área de criação das partículas
    // {
    //     P.X = pos;
    //     P.V = vec2(-0.1);//0.5*Dir(-PI*0.25 - PI*0.5);// + 0.3*sin(0.9*time));
        
    //     vec3 aux = vec3(0.0,0.0,1.0);
    //     P.M = vec4(aux,1.0);//mix(P.M, vec4(aux, fluid_rho), 0.5);
    // }

    // if(length(P.X - R*vec2(0.22, 0.5)) < 20.)//se estiver dentro do alcance da área de criação das partículas
    // {
    //     P.X = pos;
    //     P.V = vec2(-0.1);//0.5*Dir(-PI*0.25 + 0.3*sin(0.3*time));
    //     vec3 aux = vec3(0.0,1.0,0.0);
    //     P.M = vec4(aux,1.0);//mix(P.M, vec4(aux, fluid_rho), 0.5);
    // }

    if(info.z>0. && (info.y!=0. && info.x==0.)){
        P.X = pos;
        P.V = -info.xy;
        vec3 aux = vec3(0.0,1.0,0.0);
        P.M = vec4(aux,info.z);
    }

    // vec4 pixel = texture(borda, posV);
    // if(pixel.x <0.1 && pixel.y <0.1 && pixel.z <0.1){
    //     P.M = vec4(0.);
    // }
    
    vec4[2] aux = saveParticle(P, pos);
    
    //U = vec4(clamp(P.X - pos, vec2(-0.5), vec2(0.5)),P.V);
    
    U = aux[0];
    ColorMass = aux[1];
    // U=vec4(1.0);
    // ColorMass = vec4(0.0);
}
