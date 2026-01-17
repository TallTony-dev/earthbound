#version 430

#define SIMWIDTH 1200
#define SIMHEIGHT 800


//MUST MATCH fluidsim.hpp
struct FluidTile {
    float pressure;
    float velocityY;
    float velocityX;
    float r;
    float g;
    float b;
};

layout(std430, binding = 1) readonly restrict buffer SimBufferLayout {
    FluidTile tileBuffer[]; // tileBuffer[x, y] = tileBuffer[x + SIMWIDTH * y]
};

layout(std430, binding = 3) writeonly restrict buffer SimBufferLayout {
    FluidTile tileBufferDest[]; // tileBuffer[x, y] = tileBuffer[x + SIMWIDTH * y]
};

#define getTileBuffer(x, y) (tileBuffer[((x) + SIMWIDTH * (y))])
#define getTileBufferDest(x, y) (tileBufferDest[((x) + SIMWIDTH * (y))])


void main() {
    uint x = gl_GlobalInvocationID.x;
    uint y = gl_GlobalInvocationID.y;

    getTileBufferDest(x,y) = getTileBuffer(x,y);
}