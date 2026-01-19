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
    float viscosity;
} FluidSimUpdateCmd;

#define MAXFLUIDSIMTRANSFERS 6
typedef struct FluidSimUpdateBuf {
    unsigned int count;
    FluidSimUpdateCmd commands[MAXFLUIDSIMTRANSFERS];
} FluidSimUpdateBuf;


class FluidSim {
    public:
        FluidSim(FluidSimType type);
        void UpdateSim();
        void DrawSim();
    private:
        FluidSimType type_;
        float remainingTime_;
        Shader fluidSimLogicShader;
        Shader fluidSimTransferShader;
        Shader fluidSimRenderShader;
        FluidSimUpdateBuf transferBuffer;
        RenderTexture renderTexture1;
        RenderTexture renderTexture2;
        int transferShaderCommandLoc;
        int transferShaderCountLoc;
        int rendererShaderResLoc;
};