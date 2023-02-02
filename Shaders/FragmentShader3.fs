#version 460 core
in vec2 aTexCoord;
in vec3 aColor;
in vec4 aPosition;
in vec4 aMouse;
out vec4 FragColor;
uniform sampler2D Texture;
uniform vec2 iResolution;
#define LOOKUP(COORD) texture(Texture,(COORD)/iResolution.xy)

vec4 Field (vec2 position) {
    // Rule 1 : All My Energy transates with my ordered Energy
    vec2 velocityGuess = LOOKUP (position).xy;
    vec2 positionGuess_d = position - velocityGuess;
    vec2 positionGuess_e = position + velocityGuess;
    vec2 positionGuess = 0.5*(positionGuess_d +positionGuess_e);
        return LOOKUP (positionGuess);
}
void main()
{
vec2 Me = gl_FragCoord.xy;
//Me.y=iResolution.y-Me.y;
vec4 Energy = Field(Me);
vec4 pX  =  Field(Me + vec2(1,0));
//vec4 pXY  =  Field(M e + vec2(1,1));
//vec4 pX_Y  =  Field(Me + vec2(1,-1));
vec4 pY  =  Field(Me + vec2(0,1));
vec4 nX  =  Field(Me - vec2(1,0));
//vec4 nXY  =  Field(Me - vec2(1,1));
//vec4 nX_Y  =  Field(Me - vec2(1,-1));
vec4 nY  =  Field(Me - vec2(0,1));

// Rule 2 : Disordered Energy diffuses completely :
Energy.z = (pX.z + pY.z + nX.z + nY.z)/4.0;

// Rule 3 : Order in the disordered Energy creates Order :
vec2 Force;
Force.x = (nX.z - pX.z);//+(nXY.b-pXY.b);
Force.y = (nY.z - pY.z);
Energy.xy += Force/4.0;

// Rule 4 : Disorder in the ordered Energy creates Disorder :
Energy.z += (nY.y - pY.y + nX.x - pX.x )/4.;

// Gravity effect :
Energy.y -= Energy.w/300.0;

// Mass concervation :

Energy.w += (nX.x*nX.w-pX.x*pX.w+nY.y*nY.w-pY.y*pY.w)/4.0;

float D = distance(Me,vec2((1.0+aMouse.x)*iResolution.x*0.5,(1.0-aMouse.y)*0.5*iResolution.y));

Energy.w = (D<10)  && (aMouse.z>0) ? 1.0: Energy.w;

if(Me.x<10.||Me.y<10.||iResolution.x-Me.x<10.||iResolution.y-Me.y<10.)
{
    Energy.xy*=0.;
}
FragColor = Energy;
}
