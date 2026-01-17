#version 430

#define SIMWIDTH 1200 //MUST MATCH fluidsim.cpp
#define SIMHEIGHT 800 //MUST MATCH fluidsim.cpp

//MUST MATCH fluidsim.hpp
struct UpdateCommand {
    uint x;
    uint y;
    uint w;
    int xVector; //delta value to indicate velocity change
    int yVector;
    float pressure;
    float r;
    float g;
    float b;
};

//MUST MATCH fluidsim.hpp
struct FluidTile {
    float pressure;
    float velocityY;
    float velocityX;
    float r;
    float g;
    float b;
};

//output grid buffer
layout(std430, binding = 1) buffer SimBufferLayout {
    FluidTile tileBuffer[]; // tileBuffer[x, y] = tileBuffer[x + SIMWIDTH * y]
};

//command buffer
layout(std430, binding = 3) readonly restrict buffer UpdateLayout {
    uint count;
    UpdateCommand commands[];
};

#define getTileBuffer(x, y) (tileBuffer[((x) + SIMWIDTH * (y))])
#define isInside(x, y) (((x) >= 0) && ((y) >= 0) && ((x) < SIMWIDTH) && ((y) < SIMHEIGHT))

#define VECTORSTRENGTHMULT 2

void main() {
    uint cmdIndex = gl_GlobalInvocationID.x;
    if (cmdIndex >= count) return;

    UpdateCommand cmd = commands[cmdIndex];
    int radius = int(cmd.w);

    for (int x = int(cmd.x) - radius; x <= int(cmd.x) + radius; x++) {
        for (int y = int(cmd.y) - radius; y <= int(cmd.y) + radius; y++) {
            if (isInside(x, y)) {
                FluidTile tile = getTileBuffer(x, y);
                tile.pressure += cmd.pressure;
                tile.velocityX += float(cmd.xVector) * VECTORSTRENGTHMULT;
                tile.velocityY += float(cmd.yVector) * VECTORSTRENGTHMULT;
                float partialPressure = cmd.pressure / max(tile.pressure, 0.0001);
                tile.r += partialPressure * cmd.r;
                tile.g += partialPressure * cmd.g;
                tile.b += partialPressure * cmd.b;
                getTileBuffer(x, y) = tile;
            }
        }
    }
}