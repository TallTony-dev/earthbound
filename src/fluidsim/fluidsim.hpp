#pragma once
#include "fluidsimtypes.hpp"
#include "../../raylib/src/raylib.h"

//MUST MATCH GLSL
typedef struct FluidSimUpdateCmd {
    unsigned int x;
    unsigned int y;
    unsigned int w;
    int xVector; //delta value to indicate velocity change
    int yVector;
    float pressure;
    float r; //between 0 and 1 likely good
    float g;
    float b;
} FluidSimUpdateCmd;

#define MAXFLUIDSIMTRANSFERS 6
typedef struct FluidSimUpdateSSBO {
    unsigned int count;
    FluidSimUpdateCmd commands[MAXFLUIDSIMTRANSFERS];
} FluidSimUpdateSSBO;

//MUST MATCH GLSL
typedef struct FluidTile {
    float pressure;
    float velocityY;
    float velocityX;
    float r;
    float g;
    float b;
} FluidTile;



class FluidSim {
    public:
        FluidSim(FluidSimType type);
        void UpdateSim();
        void DrawSim();
    private:
        FluidSimType type_;
        float remainingTime_;
        unsigned int fluidSimLogicProgram;
        unsigned int fluidSimTransferProgram;
        Shader fluidSimRenderShader;
        unsigned int ssboA;
        unsigned int ssboB;
        unsigned int ssboTransfer;
        int resUniformLoc;
        Vector2 resolution;
        FluidSimUpdateSSBO transferBuffer;
};