#version 460 core

uniform sampler2D bufferA;
uniform sampler2D cor;
uniform sampler2D bufferB;
uniform sampler2D bufferC;
uniform sampler2D stop;

in vec2 R;
in float time;
in vec4 mouse;
in vec2 posV;


out vec4 col;

#include <common.fs>

vec3 hsv2rgb( in vec3 c )
{
    vec3 rgb = clamp( abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );

	rgb = rgb*rgb*(3.0-2.0*rgb); // cubic smoothing	

	return c.z * mix( vec3(1.0), rgb, c.y);
}

vec3 mixN(vec3 a, vec3 b, float k)
{
    return sqrt(mix(a*a, b*b, clamp(k,0.,1.)));
}

vec4 V(vec2 p)
{
    return pixel(bufferC, p);
}

void main()
{
    //pos = R*0.5 + pos*0.1;

     

    vec2 pos = gl_FragCoord.xy;


    ivec2 p = ivec2(pos);
    
    vec4 data = texel(bufferA, pos);
    vec4 massa = texel(cor, pos);
    particle P = getParticle(data, massa, pos);
    //P.M *= P.M.w;
    
    //border render
    vec3 Nb = bN(P.X);
    float bord = smoothstep(2.*border_h,border_h*1.0,border(pos));
    
    // vec4 rho = V(pos);
    // //vec3 dx = vec3(-2., 0., 2.);
    // //vec4 grad = -0.5*vec4(V(pos + dx.zy).zw - V(pos + dx.xy).zw,
    // //                     V(pos + dx.yz).zw - V(pos + dx.yx).zw);
    // //vec2 N = pow(length(grad.xz),0.2)*normalize(grad.xz+1e-5);
    // //float specular = pow(max(dot(N, Dir(1.4)), 0.), 3.5);
    // float specularb = G(0.4*(Nb.zz - border_h))*pow(max(dot(Nb.xy, Dir(1.4)), 0.), 3.);
    
    // float a = pow(smoothstep(fluid_rho*0., fluid_rho*2., rho.z),0.1);
    // float b = exp(-1.7*smoothstep(fluid_rho*1., fluid_rho*7.5, rho.z));
    // vec3 col0 = vec3(1., 0.5, 0.);
    // vec3 col1 = vec3(0.1, 0.4, 1.);
	// vec3 fcol = mixN(col0, col1, tanh(3.*(rho.w - 0.7))*0.5 + 0.5);
    // // Output to screen
    // col = vec4(3.);
    // col.xyz = mixN(col.xyz, fcol.xyz*(1.5*b + specularb*5.), a);
    // col.xyz = mixN(col.xyz, 0.*vec3(0.7,0.7,1.), bord);
    // col.xyz = tanh(col.xyz);
    //vec4 data = texel(bufferC, pos); 
    vec4 pixel = texture(stop, posV);
    vec4 pixel2 = texture(stop, posV-vec2(0.0001)/R);
    vec4 rtn = vec4(0.);
    if(pixel.xyz == vec3(0.)){
        rtn = vec4(1.,0.,0.,0.);
    }
    rtn+= P.M*P.M.w;

    // if(pixel2 == pixel){
    //     rtn = vec4(1.);
    // }else{
    //     rtn = vec4(0.);
    // }

    col = rtn;
    //col += abs(vec4(bord)) +P.M*P.M.w;
    //col = texture(stop,posV);
    


    //vec4 rtn = texel(cor, pos);
    
}
