#version 330 core

out vec4 fragColor;

in vec3 Normal;
in vec3 fragPos;
in vec2 TexCoord;

uniform sampler2D textura;

void main(){
	fragColor=texture(textura,TexCoord);
}