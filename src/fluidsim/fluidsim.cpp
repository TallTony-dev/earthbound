#include "fluidsim.hpp"
#include "fluidsimtypes.hpp"
#include "../../raylib/src/raylib.h"
#include "../../raylib/src/rlgl.h"
#include <algorithm>
#include <iostream>

#define SIMWIDTH 1200 //MUST BE THE SAME AS IN both GLSLs and / 16
#define SIMHEIGHT 1200 //MUST BE THE SAME AS IN both GLSLs and / 16

//Fluid sim development largely supported by raylib's 'game of life' example
//which shows how to use compute shaders in the context of raylib.
void FluidSim::UpdateSim() {
    //float deltaTime = GetFrameTime();
    if (transferBuffer.count < MAXFLUIDSIMTRANSFERS) {
        if (type_ == type1) {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                transferBuffer.commands[transferBuffer.count].x = GetMouseX();
                transferBuffer.commands[transferBuffer.count].y = GetMouseY();
                Vector2 delta = GetMouseDelta();
                transferBuffer.commands[transferBuffer.count].xVector = delta.x;
                transferBuffer.commands[transferBuffer.count].yVector = delta.y;

                transferBuffer.commands[transferBuffer.count].w = 20;

                transferBuffer.commands[transferBuffer.count].viscosity = 1.0f;

                //transferBuffer.commands[transferBuffer.count].pressure = 0.2f;
                transferBuffer.count++;
            }
        }
    }


    //run commands from buffer
    if (transferBuffer.count > 0) {
        float commands[MAXFLUIDSIMTRANSFERS * 7]; //to go to the shader
        for (int i = 0; i < transferBuffer.count; i++) {
            commands[i * 7 + 0] = transferBuffer.commands[i].x;
            commands[i * 7 + 1] = transferBuffer.commands[i].y;
            commands[i * 7 + 2] = transferBuffer.commands[i].w;
            commands[i * 7 + 3] = transferBuffer.commands[i].xVector;
            commands[i * 7 + 4] = transferBuffer.commands[i].yVector;
            commands[i * 7 + 5] = transferBuffer.commands[i].pressure;
            commands[i * 7 + 6] = transferBuffer.commands[i].viscosity;
        }
        

        //run transfer shader
        BeginTextureMode(renderTexture2);
        BeginShaderMode(fluidSimTransferShader);
        SetShaderValue(fluidSimTransferShader, transferShaderCountLoc, &transferBuffer.count, SHADER_UNIFORM_INT);
        SetShaderValueV(fluidSimTransferShader, transferShaderCommandLoc, commands, SHADER_UNIFORM_FLOAT, MAXFLUIDSIMTRANSFERS * 7); //set command uniform
        DrawTexturePro(renderTexture1.texture, Rectangle{0,0,SIMWIDTH,SIMHEIGHT}, Rectangle{0,0,SIMWIDTH,SIMHEIGHT}, {0,0}, 0, PINK);
        EndShaderMode();
        EndTextureMode();
        //now commands have been drawn to renderTexture2 using renderTexture1 as the previous state

        transferBuffer.count = 0;
    }


    //step simulation here
    BeginTextureMode(renderTexture1);
    BeginShaderMode(fluidSimLogicShader);
    DrawTexturePro(renderTexture2.texture, Rectangle{0,0,SIMWIDTH,SIMHEIGHT}, Rectangle{0,0,SIMWIDTH,SIMHEIGHT}, {0,0}, 0, GREEN);
    EndShaderMode();
    EndTextureMode();
    //now renderTexture1 has the correct state after logic is applied
    std::swap(renderTexture1, renderTexture2);
}

void FluidSim::DrawSim() {
    BeginShaderMode(fluidSimRenderShader);
    Vector2 res{(float)GetScreenWidth(), (float)GetScreenHeight()};
    SetShaderValue(fluidSimRenderShader, rendererShaderResLoc, &res, SHADER_UNIFORM_VEC2);
    DrawTexturePro(renderTexture1.texture, {0,0,SIMWIDTH,SIMHEIGHT}, {0,0,(float)GetScreenWidth(), (float)GetScreenHeight()}, {0,0}, 0, PURPLE);
    EndShaderMode();
}

FluidSim::FluidSim(FluidSimType type) {
    type_ = type;

    fluidSimLogicShader = LoadShader(0, "../resources/fluidshaders/fluidsimlogic.glsl");
    fluidSimTransferShader = LoadShader(0, "../resources/fluidshaders/fluidsimtransfer.glsl");

    if (type == type1) {
        fluidSimRenderShader = LoadShader(0,"../resources/fluidshaders/fluidsimrenderer1.glsl");
    }

    transferShaderCommandLoc = GetShaderLocation(fluidSimTransferShader, "commands");
    transferShaderCountLoc = GetShaderLocation(fluidSimTransferShader, "commandCount");

    rendererShaderResLoc = GetShaderLocation(fluidSimRenderShader, "resolution");

    renderTexture1 = LoadRenderTexture(SIMWIDTH, SIMHEIGHT);
    renderTexture2 = LoadRenderTexture(SIMWIDTH, SIMHEIGHT);
    
    // Clear render textures to black
    BeginTextureMode(renderTexture1);
    ClearBackground({101,(unsigned char)GetRandomValue(0,255),0,0});
    EndTextureMode();
    BeginTextureMode(renderTexture2);
    ClearBackground({101,(unsigned char)GetRandomValue(0,255),0,0});
    EndTextureMode();
    
    transferBuffer = {0};
    //test
}