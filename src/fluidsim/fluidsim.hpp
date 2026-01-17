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

#define MAXFLUIDSIMTRANSFERS 6 //match glsl
typedef struct FluidSimUpdateSSBO {
    unsigned int count;
    FluidSimUpdateCmd commands[MAXFLUIDSIMTRANSFERS * 3]; //12 floats per thing, last 3 unused
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
        Shader fluidSimTransferShader;
        Shader fluidSimRenderShader;
        Shader fluidSimLogicShader;
        // unsigned int ssboA;
        // uns
        RenderTexture2D stateA_part1;
        RenderTexture2D stateA_part2;
        RenderTexture2D stateB_part1;
        RenderTexture2D stateB_part2;
        bool useStateA; 

        int transferOutputPartLoc;
        int logicOutputPartLoc;
        int resUniformLoc;
        int transferCountUniformLoc;
        FluidSimUpdateSSBO transferBuffer;
};