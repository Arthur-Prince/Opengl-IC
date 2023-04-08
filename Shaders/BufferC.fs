#version 460 core

uniform sampler2D bufferA;
uniform sampler2D cor;
in vec2 R;
in float time;
in vec4 mouse;

out vec4 fragColor;
out vec4 massa;

#include <common.fs>

void main()
{
    vec2 pos = gl_FragCoord.xy;
    ivec2 p = ivec2(pos);

    vec4 data = texel(bufferA, pos);
    vec4 massa = texel(cor, pos);
    particle P = getParticle(data, massa, pos);
    
    //particle render
    vec4 rho = vec4(0.);
    range(i, -1, 1) range(j, -1, 1)
    {
        vec2 ij = vec2(i,j);
        vec4 data = texel(bufferA, pos + ij);
        vec4 massa = texel(cor, pos + ij);
        particle P0 = getParticle(data, massa, pos + ij);

        vec2 x0 = P0.X; //update position
        //how much mass falls into this pixel
        // rho += 1.*vec4(P.V, P.M.xy)*G((pos - x0)/0.75); 
        rho += 1.*vec4(P.M)*G((pos - x0)/0.75); 
    }
    
    fragColor = rho;
    massa = texel(cor, pos);

}
