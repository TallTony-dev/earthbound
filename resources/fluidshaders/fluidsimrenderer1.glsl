#version 330 core

#define SIMWIDTH 1200
#define SIMHEIGHT 800

uniform vec2 resolution;
uniform sampler2D texture0;        // Part1 - bound by DrawTexturePro
uniform sampler2D tileBufferPart2; // Part2 - manually bound

in vec2 fragTexCoord; //also recall gl_fragcoord
out vec4 finalColor;

void main() {
    // Use the texture coordinates from vertex shader
    vec2 texCoord = fragTexCoord;
    
    vec4 part1 = texture(texture0, texCoord);
    vec4 part2 = texture(tileBufferPart2, texCoord);
    
    float r = part1.w;
    float g = part2.x;
    float b = part2.y;
    
    // Show dark blue background when no fluid data (helps with debugging)
    if (r < 0.001 && g < 0.001 && b < 0.001) {
        finalColor = vec4(0.05, 0.05, 0.15, 1.0);
    } else {
        finalColor = vec4(r, g, b, 1.0);
    }
}