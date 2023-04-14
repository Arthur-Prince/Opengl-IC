#define Bf(p) p
#define Bi(p) ivec2(p)
#define texel(a, p) texelFetch(a, Bi(p), 0)
#define pixel(a, p) texture(a, (p)/R)

#define PI 3.14159265

#define loop(i,x) for(int i = 0; i < x; i++)
#define range(i,a,b) for(int i = a; i <= b; i++)

#define dt 0.5

#define border_h 5.

#define mass 0.5

#define fluid_rho 0.5



float Pf(vec4 rho)
{
    //return 0.2*rho.x; //gas
    float GF = 1.;//smoothstep(0.49, 0.5, 1. - rho.y);
    return mix(0.5*rho.w,0.04*rho.w*(rho.w/fluid_rho - 1.), GF); //water pressure
}

mat2 Rot(float ang)
{
    return mat2(cos(ang), -sin(ang), sin(ang), cos(ang)); 
}

vec2 Dir(float ang)
{
    return vec2(cos(ang), sin(ang));
}


float sdBox( in vec2 p, in vec2 b )
{
    vec2 d = abs(p)-b;
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
}

float border(vec2 p)
{
    float bound = -sdBox(p - R*0.5, R*vec2(0.49, 0.49)); 
    float box = sdBox(Rot(0.*time)*(p - R*vec2(1.5, 0.6)) , R*vec2(0.05, 0.01));
    float drain = -sdBox(p - R*vec2(0.5, 2.0), R*vec2(1.5, 0.5));
    return max(drain,min(bound, box));
}

#define h 1.
vec3 bN(vec2 p)
{
    vec3 dx = vec3(-h,0,h);
    vec4 idx = vec4(-1./h, 0., 1./h, 0.25);
    vec3 r = idx.zyw*border(p + dx.zy)
           + idx.xyw*border(p + dx.xy)
           + idx.yzw*border(p + dx.yz)
           + idx.yxw*border(p + dx.yx);
    return vec3(normalize(r.xy), r.z + 1e-4);
}

struct particle
{
    vec2 X;
    vec2 V;
    vec4 M;
};
    
particle getParticle(vec4 data, vec4 massa, vec2 pos)
{
    particle P; 
    P.X = vec2(pos+ data.xy);
    P.V = data.zw;
    P.M = massa;
    return P;
}

vec4[2] saveParticle(particle P, vec2 pos)
{
    vec2 c  = clamp(P.X - pos, vec2(-0.5), vec2(0.5));
    vec4[2] rtn;
    rtn[0] = vec4(c, P.V);
    rtn[1] = P.M;
    return rtn;
}

vec3 hash32(vec2 p)
{
	vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yxz+33.33);
    return fract((p3.xxy+p3.yzz)*p3.zyx);
}

float G(vec2 x)
{
    return exp(-dot(x,x));
}

float G0(vec2 x)
{
    return exp(-length(x));
}

//diffusion amount
#define dif 1.12

vec3 distribution(vec2 x, vec2 p, float K)
{
    vec2 omin = clamp(x - K*0.5, p - 0.5, p + 0.5);
    vec2 omax = clamp(x + K*0.5, p - 0.5, p + 0.5); 
    return vec3(0.5*(omin + omax), (omax.x - omin.x)*(omax.y - omin.y)/(K*K));
}

float waterDiffusionRadius(float massa){
    float difR = 0.001;
    return difR;
}

vec2 dirUnitaria(vec2 x){
    vec2 aux = abs(x);
    vec2 rtn = vec2(0.);
    if (x.x != 0.) {
        rtn.x = x.x/aux.x;
    } 
    if (x.y != 0.) {
        rtn.y = x.y/aux.y;
    } 
    return rtn;
}

bool ePreto(vec4 cor){
    if(cor.x ==0. && cor.y ==0. && cor.z ==0.) 
    {
        return true;
    }
    return false;
}

void cantoNormal(vec4 cor , inout vec2 dir){
    if(ePreto(cor)){
        dir *=-1.;
    }
}

void rebate(sampler2D info, inout particle P, vec2 pos){
    
}


void fBorda(sampler2D info, inout particle P, vec2 pos) {
    // porcentagem de tempo
    
    // vec4 nextPos = texture(info, (P.X+P.V)/R);
    // vec4 eu = texture(info,pos/R);

    // vec2 time = (clamp(P.X+P.V, pos + vec2(-0.5), pos + vec2(0.5)) - P.X)/P.V;

    // vec2 sinal = dirUnitaria(P.V);

    vec2 x = P.X+P.V;
    float K = waterDiffusionRadius(P.M.w);

    // vec2 canto = x + K*sinal;

    vec2 cantodir[4] = vec2[](vec2(1.,1.), vec2(1.,-1.), vec2(-1.,1.), vec2(-1.,-1.));

    vec4 cantoCor[4];

    vec2 normalBorda = vec2(0., 0.);

    loop(i, 4){
        cantoCor[i] = texture(info, (x+K*cantodir[i])/R);
        cantoNormal(cantoCor[i], cantodir[i]);
        normalBorda += cantodir[i];

    }
    
    normalize(normalBorda);

    P.V = reflect(P.V, normalBorda);
    




    // vec2 omin = clamp(x - K*0.5, pos - 0.5, pos + 0.5);
    // vec2 omax = clamp(x + K*0.5, pos - 0.5, pos + 0.5); 

    // vec2 difPos = pos -0.5*(omin + omax);

    // //posicao do canto do lado de fora
    // vec2 outbord = x+difPos;


    //vec2 dir = (((K+P.V)/K - K)*K);

    //vec2 dir = dirUnitaria(difPos);

    
    
    // vec4 vert = texture(info, (pos+vec2(dir.x, 0.))/R);
    // vec4 horiz = texture(info, (pos+vec2(0., dir.y))/R);

    // if(ePreto(horiz)){
    //     P.V.x = -(P.V.x-difPos.x);
    //     P.X.x += difPos.x;
    // }

    // if(ePreto(vert)){
    //     //P.M =vec4(0.);
    //     P.V.y = -(P.V.y-difPos.y);
    //     P.X.y += difPos.y;
    // }
    

    // vec4 eu = texture(info, (pos)/R);
    // if(ePreto(eu)){
    //     P.V = vec2(0.);
    // }


    
}




/*vec3 distribution(vec2 x, vec2 p, float K)
{
    vec4 aabb0 = vec4(p - 0.5, p + 0.5);
    vec4 aabb1 = vec4(x - K*0.5, x + K*0.5);
    vec4 aabbX = vec4(max(aabb0.xy, aabb1.xy), min(aabb0.zw, aabb1.zw));
    vec2 center = 0.5*(aabbX.xy + aabbX.zw); //center of mass
    vec2 size = max(aabbX.zw - aabbX.xy, 0.); //only positive
    float m = size.x*size.y/(K*K); //relative amount
    //if any of the dimensions are 0 then the mass is 0
    return vec3(center, m);
}*/







