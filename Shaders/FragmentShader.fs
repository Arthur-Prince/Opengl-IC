#version 460 core
in vec4 aPosition;
out vec4 FragColor;
uniform sampler2D Texture;
uniform vec2 iResolution;
#define LOOKUP(COORD) texture(Texture,(COORD)/iResolution.xy)
void main()
{
   vec2 Me= gl_FragCoord.xy;
   FragColor = vec4(LOOKUP(Me).xyz,1.0);
}
