#version 400

out vec4 frag_colour;
in vec3 pos;

uniform samplerCube sampler0;
uniform vec4 col;
void main(){
    frag_colour = col;
}
