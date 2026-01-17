# version 330 core

precision mediump float;
//check config.h for what values are preset by raylib
// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord; //also recall gl_fragcoord
in vec4 fragColor;
in vec3 fragNormal;
uniform float totalTime;
uniform sampler2D texture0;

out vec4 finalColor;

void main() {
    FragColor = vec4(1.0, 0.0, 0.0, 1.0); 
}