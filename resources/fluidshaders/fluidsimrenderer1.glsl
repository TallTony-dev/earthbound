#version 430

#define SIMWIDTH 1200 //MUST MATCH fluidsim.cpp
#define SIMHEIGHT 800 //MUST MATCH fluidsim.cpp

in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

uniform vec2 resolution;

out vec4 finalColor;

struct FluidTile {
    float pressure;
    float velocityY;
    float velocityX;
    float r;
    float g;
    float b;
};

layout(std430, binding = 1) buffer SimBufferLayout {
    FluidTile tileBuffer[]; // tileBuffer[x, y] = tileBuffer[x + SIMWIDTH * y]
};
#define getTileBuffer(x, y) (tileBuffer[((x) + SIMWIDTH * (y))])


void main() {
    ivec2 uv = ivec2(int((float(SIMWIDTH) / resolution.x) * fragTexCoord.x),
                    int((float(SIMHEIGHT) / resolution.y) * fragTexCoord.y));
    uv = clamp(uv, ivec2(0), ivec2(SIMWIDTH - 1, SIMHEIGHT - 1));

    FluidTile tile = getTileBuffer(uv.x, uv.y);
    finalColor = vec4(tile.r, tile.g, tile.b, 1.0);
}