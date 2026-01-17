#version 330 core

#define SIMWIDTH 1200
#define SIMHEIGHT 800

// Input from vertex shader
in vec2 fragTexCoord;

out vec4 fragColor;

uniform sampler2D texture0;        // Part1 - bound by DrawTexturePro
uniform sampler2D tileBufferPart2; // Part2 - manually bound
uniform int outputPart; // 0 -> part1, 1 -> part2

void main() {
    vec2 uv = fragTexCoord;
    vec4 part1 = texture(texture0, uv);
    vec4 part2 = texture(tileBufferPart2, uv);

    if (outputPart == 0) fragColor = part1;
    else fragColor = part2;
}