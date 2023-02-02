#version 460 core

in vec3 cor;
in vec2 Coord;
layout (location = 0)out vec4 f1;
layout (location = 1)out vec4 f2;

uniform int Periodo;
uniform sampler2D Texture;
uniform sampler2D Texture2;
uniform vec4 mouse;
uniform vec2 iResolution;


struct particle
{
    vec2 pos;
    vec2 speed;
    float mass;
};

vec2[25] textVizMatriz5(){
    int n=5;
    vec2[25] rtn;
    for(int i =0; i<5; i++){
        for(int j =0; j<5; j++){
            rtn[j + i*5] = vec2(i-2,j-2);
        }
    }
    return rtn;
}

vec2[9] textVizMatriz3(){
    int n=3;
    vec2[9] rtn;
    for(int i =0; i<n; i++){
        for(int j =0; j<n; j++){
            rtn[j + i*n] = vec2(i-1,j-1);
        }
    }
    return rtn;
};

float[9] kernelMediaSD(){
    float[9] rtn = {0.0,      1.0/4,    0.0,
                    1.0/4,    0.0,      1.0/4,
                    0.0,      1.0/4,    0.0};
    return rtn;
};

float[9] kernelMediaSMe(){
    float[9] rtn = {1.0/8,    1.0/8,    1.0/8,
                    1.0/8,    0.,       1.0/8,
                    1.0/8,    1.0/8,    1.0/8};
    return rtn;
};

float[9] kernelMedia(){
    float[9] rtn = {1.0/9,    1.0/9,    1.0/9,
                    1.0/9,    1.0/9,    1.0/9,
                    1.0/9,    1.0/9,    1.0/9};
    return rtn;
};

float[9] kernelGausianoSMe(){
    float[9] rtn = {1.0/12,    2.0/12,    1.0/12,
                    2.0/12,    0.0,       2.0/12,
                    1.0/12,    2.0/12,    1.0/12};
    return rtn;
};

float[9] kernelGausiano(){
    float[9] rtn = {1.0/16,    2.0/16,    1.0/16,
                    2.0/16,    4.0/16,    2.0/16,
                    1.0/16,    2.0/16,    1.0/16};
    return rtn;
};

float[25] kernelGausiano5(){
    float[25] rtn ={1.0/273,    4.0/273,    7.0/273,     4.0/273,     1.0/273,
                    4.0/273,    16.0/273,   26.0/273,    16.0/273,    4.0/273,
                    7.0/273,    26.0/273,   41.0/273,    26.0/273,    7.0/273,
                    4.0/273,    16.0/273,   26.0/273,    16.0/273,    4.0/273,
                    1.0/273,    4.0/273,    7.0/273,     4.0/273,     1.0/273 
                    };
    return rtn;
};

void MeAndMyNeighborhood(){
    //config
    int n = 3;
    
    float kernel[9] = kernelMediaSD();
    vec2 vizinhos[9] = textVizMatriz3();

    int eu = int(n*n/2 + 0.5);
    vec4 pixels[9];

    //code
    for(int i =0; i<n*n; i++){
        pixels[i] = texture(Texture, (Coord) + vizinhos[i]/iResolution);
    }

    vec4 Energy = pixels[eu];
    
    // Rule 2 : Disordered Energy diffuses completely :

    float media=0;
    for(int i =0; i<n*n; i++){
        media += pixels[i].z * kernel[i];
    }
    Energy.z = media;

    // Rule 3 : Order in the disordered Energy creates Order :
    vec2 Force = vec2(0.,0.);
    
    float massa = 0;

    for(int i =0; i<n*n; i++){
        if(i==eu){break;}
        float dist = distance(vec2(0,0), vizinhos[i]);
        Force.x = pixels[i].z*kernel[i]*vizinhos[i].x ;//dist;
        Force.y = pixels[i].z*kernel[i]*vizinhos[i].y ;//dist;
        Energy.xy += Force;


        // Rule 4 : Disorder in the ordered Energy creates Disorder :
        float disordered = ((pixels[i].x*vizinhos[i].x) + (pixels[i].y * vizinhos[i].y))*kernel[i] /dist;;

        Energy.z -= disordered;
        massa += disordered * pixels[i].w;

        
    }
    
    //pula o proprio pixel para n dar divizao por 0


    // Gravity effect :
    Energy.y -= Energy.w/300.0;

    // Mass concervation :

    Energy.w -= massa;

    vec2 Me = gl_FragCoord.xy;

    // vec4 Energy = texture(Texture, (Coord));
    // vec4 nX = texture(Texture, (Coord)+vec2(-1,0)/iResolution);
    // vec4 pX = texture(Texture, (Coord)+vec2(1,0)/iResolution);
    // vec4 pY = texture(Texture, (Coord)+vec2(0,1)/iResolution);
    // vec4 nY = texture(Texture, (Coord)+vec2(0,-1)/iResolution);

    // // Rule 2 : Disordered Energy diffuses completely :
    // Energy.z = (pX.z + pY.z + nX.z + nY.z)/4.0;

    // // Rule 3 : Order in the disordered Energy creates Order :
    // vec2 Force;
    // Force.x = (nX.z - pX.z);
    // Force.y = (nY.z - pY.z);
    // Energy.xy += Force/4.0;

    // // Rule 4 : Disorder in the ordered Energy creates Disorder :
    // Energy.z += ((nY.y - pY.y )+ (nX.x - pX.x) )/4.;

    // // Gravity effect :
    // Energy.y -= Energy.w/300.0;

    // // Mass concervation :

    // Energy.w += (((nX.x*nX.w)-(pX.x*pX.w))+((nY.y*nY.w)-(pY.y*pY.w)));

    float D = distance(Me,vec2((1.0+mouse.x)*iResolution.x*0.5,(1.0-mouse.y)*0.5*iResolution.y));

    Energy.w = (D<10.0)  && (mouse.z>0) ? 1.0: Energy.w;

    if(Me.x<10.||Me.y<10.||iResolution.x-Me.x<10.||iResolution.y-Me.y<10.)
    {
        Energy.xy*=0.;
    }

    f1 = Energy.wwww;
    f2 = Energy.wwww;




}; 


void SPH(){

    //config
    int n = 3;
    
    float kernel[9] = kernelMedia();
    vec2 vizinhos[9] = textVizMatriz3();

    int eu = int(n*n/2 + 0.5);
    vec2 Me = gl_FragCoord.xy;
    vec4 pixels[9];
    particle rtn;
    rtn.pos = vec2(0.);
    rtn.speed = vec2(0.);
    rtn.mass = 0.;

    particle particulas[9];

    //code
    for(int i =0; i<n*n; i++){
        // pixels[i] = texture(Texture, (Coord) + (vizinhos[i]/iResolution));
        vec4 text1 = texture(Texture, (Coord) + vizinhos[i]/iResolution);
        vec4 text2 = texture(Texture2, (Coord) + vizinhos[i]/iResolution);
        particulas[i].pos = text1.xy*iResolution;
        particulas[i].speed = text2.xy*iResolution;
        particulas[i].mass = text2.z;
    }

    //para as particulas na vizinhança
    for(int i =0; i<n*n; i++){
        particle atual = particulas[i];

        //atualiza posicao
        atual.pos += atual.speed*iResolution;//*periodo

        if(abs(atual.pos.x-Me.x)<1.0 && abs(atual.pos.y-Me.y)<1.0){
            rtn.pos += atual.pos * atual.mass * kernel[i];
            rtn.speed += atual.speed * atual.mass * kernel[i];
            rtn.mass += atual.mass * kernel[i];
        }
        

    }


    //normaliza
    if(rtn.mass>0.01){
        rtn.pos /= rtn.mass;
        rtn.speed /= rtn.mass;
    }

    //retorna
    float D = distance(Me,vec2((1.0+mouse.x)*iResolution.x*0.5,(1.0-mouse.y)*0.5*iResolution.y));
    if((D<10.0)  && (mouse.z>0)){
        rtn.pos = Coord;
        rtn.speed = vec2(0.0,-1.0);
        rtn.mass = 1.0;

    }
    f1= vec4(rtn.pos.xy/iResolution,1.0,0.0);
    f2= vec4(rtn.speed/iResolution,rtn.mass,1.0);
    
    // float media=0.;
    // for(int i =0; i<n*n; i++){
    //     media += pixels[i].x * kernel[i];
        
    // }
    // //test
    // vec2 Me = gl_FragCoord.xy;
    // float D = distance(Me,vec2((1.0+mouse.x)*iResolution.x*0.5,(1.0-mouse.y)*0.5*iResolution.y));
    // vec4 cor = (D<100.0)  && (mouse.z>0) ? vec4(1.0,1.0,1.0,1.0) : vec4(media);
    // vec4 a = (media.x>0.9) ? vec4(1.0,1.0,0.0,0.0) : vec4(0.);

    // f1 = cor;// - pixels[eu]/30;
    // f2 = a;



};


void main(){

    SPH();



}