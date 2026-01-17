#version 330 core

#define SIMWIDTH 1200
#define SIMHEIGHT 800
#define MAX_COMMANDS 6u

// Input from vertex shader
in vec2 fragTexCoord;

out vec4 fragColor;

uniform sampler2D texture0;        // Part1 - bound by DrawTexturePro
uniform sampler2D tileBufferPart2; // Part2 - manually bound
uniform uint commandCount;
uniform vec4 commands[MAX_COMMANDS * 3u];
uniform int outputPart; // 0 = part1, 1 = part2

void main() {
    // Use fragTexCoord for both coordinate lookup and texture sampling
    // fragTexCoord is in [0,1] range, scale to pixel coords for command matching
    ivec2 tileCoord = ivec2(vec2(fragTexCoord.x,1 - fragTexCoord.y) * vec2(SIMWIDTH, SIMHEIGHT));
    vec2 uv = fragTexCoord;

    vec4 part1 = texture(texture0, uv);
    vec4 part2 = texture(tileBufferPart2, uv);

    float pressure = part1.x;
    float velocityY = part1.y;
    float velocityX = part1.z;
    float r = part1.w;
    float g = part2.x;
    float b = part2.y;

    for (uint i = 0u; i < commandCount && i < MAX_COMMANDS; i++) {
        uint index = i * 3u;
        vec4 cmd0 = commands[index];
        vec4 cmd1 = commands[index + 1u];
        vec4 cmd2 = commands[index + 2u];

        int cmdX = int(cmd0.x);
        int cmdY = int(cmd0.y);
        int radius = int(cmd0.z);
        int xVector = int(cmd0.w);
        int yVector = int(cmd1.x);
        float cmdPressure = cmd1.y;
        float cmdR = cmd1.z;
        float cmdG = cmd1.w;
        float cmdB = cmd2.x;
        
        // Check if this tile is affected
        if (abs(tileCoord.x - cmdX) <= radius && abs(tileCoord.y - cmdY) <= radius) {
            pressure += cmdPressure;
            velocityX += float(xVector) * 2.0;
            velocityY += float(yVector) * 2.0;
            
            float partialPressure = cmdPressure / max(pressure, 0.0001);
            r += partialPressure * cmdR;
            g += partialPressure * cmdG;
            b += partialPressure * cmdB;
        }
    }

    if (outputPart == 0) fragColor = vec4(pressure, velocityY, velocityX, r);
    else fragColor = vec4(g, b, 0.0, 0.0);
}
