#include "fluidsim.hpp"
#include "fluidsimtypes.hpp"
#include "../../raylib/src/raylib.h"
#include "../../raylib/src/rlgl.h"
#include <algorithm>
#include <iostream>

#define SIMWIDTH 1200 //MUST BE THE SAME AS IN both GLSLs and / 16
#define SIMHEIGHT 800 //MUST BE THE SAME AS IN both GLSLs and / 16

//Fluid sim development largely supported by raylib's 'game of life' example
//which shows how to use compute shaders in the context of raylib.
void FluidSim::UpdateSim() {
    float deltaTime = GetFrameTime();
    if (transferBuffer.count < MAXFLUIDSIMTRANSFERS) {
        if (type_ == type1) {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                transferBuffer.commands[transferBuffer.count].x = GetMouseX();
                transferBuffer.commands[transferBuffer.count].y = GetMouseY();
                Vector2 delta = GetMouseDelta();
                transferBuffer.commands[transferBuffer.count].xVector = delta.x;
                transferBuffer.commands[transferBuffer.count].yVector = delta.y;

                transferBuffer.commands[transferBuffer.count].w = 6.0f;

                transferBuffer.commands[transferBuffer.count].r = ((int)(GetTime() / 50)) % 1;
                transferBuffer.commands[transferBuffer.count].b = ((int)(GetTime() / 20)) % 1;

                transferBuffer.commands[transferBuffer.count].pressure = 2.0f;
                transferBuffer.count++;
            }
        }
    }
    //run commands from buffer
    if (transferBuffer.count > 0) {
        //update actual ssbotransfer ssbo on gpu
        rlUpdateShaderBuffer(ssboTransfer, &transferBuffer, sizeof(FluidSimUpdateSSBO), 0);

        //run transfer shader
        rlEnableShader(fluidSimTransferProgram);
        rlBindShaderBuffer(ssboA, 1);
        rlBindShaderBuffer(ssboTransfer, 3);
        rlComputeShaderDispatch(transferBuffer.count, 1, 1); //commands processed seperately?
        rlDisableShader();

        transferBuffer.count = 0;
    }

    //step simulation here
    rlEnableShader(fluidSimLogicProgram);
    rlBindShaderBuffer(ssboA, 1);
    rlBindShaderBuffer(ssboB, 3); //output to ssboB
    rlComputeShaderDispatch(SIMWIDTH / 16, SIMHEIGHT / 16, 1);
    rlDisableShader();

    std::swap(ssboA, ssboB); //ssboA to draw
}

void FluidSim::DrawSim() {
    rlBindShaderBuffer(ssboA, 1);
    resolution.x = GetScreenWidth();
    resolution.y = GetScreenHeight();
    SetShaderValue(fluidSimRenderShader, resUniformLoc, &resolution, SHADER_UNIFORM_VEC2);
    BeginShaderMode(fluidSimRenderShader);
    DrawRectangle(0,0,GetScreenWidth(), GetScreenHeight(), WHITE);
    EndShaderMode();
}

FluidSim::FluidSim(FluidSimType type) {
    type_ = type;

    char *fluidSimLogicCode = LoadFileText("../resources/fluidshaders/fluidsimlogic.glsl");
    unsigned int fluidSimLogicShader = rlCompileShader(fluidSimLogicCode, RL_COMPUTE_SHADER);
    fluidSimLogicProgram = rlLoadComputeShaderProgram(fluidSimLogicShader);
    UnloadFileText(fluidSimLogicCode);

    char *fluidSimTransferCode = LoadFileText("../resources/fluidshaders/fluidsimtransfer.glsl");
    unsigned int fluidSimTransferShader = rlCompileShader(fluidSimTransferCode, RL_COMPUTE_SHADER);
    fluidSimTransferProgram = rlLoadComputeShaderProgram(fluidSimTransferShader);
    UnloadFileText(fluidSimTransferCode);


    if (type == type1) {
        fluidSimRenderShader = LoadShader(0,"../resources/fluidshaders/fluidsimrenderer1.glsl");
        resUniformLoc = GetShaderLocation(fluidSimRenderShader, "resolution");
    }
    
    ssboA = rlLoadShaderBuffer(sizeof(FluidTile) * SIMHEIGHT * SIMWIDTH, 0, RL_DYNAMIC_COPY);
    ssboB = rlLoadShaderBuffer(sizeof(FluidTile) * SIMHEIGHT * SIMWIDTH, 0, RL_DYNAMIC_COPY);
    ssboTransfer = rlLoadShaderBuffer(sizeof(FluidSimUpdateSSBO), 0, RL_DYNAMIC_COPY);

    transferBuffer = {0};
    
}