#include "fluidsim.hpp"
#include "fluidsimtypes.hpp"
#include "../../raylib/src/raylib.h"
#include "../../raylib/src/rlgl.h"
#include <algorithm>
#include <iostream>
#include <cmath>

#define SIMWIDTH 1200 //MUST BE THE SAME AS IN both GLSLs and / 16
#define SIMHEIGHT 800 //MUST BE THE SAME AS IN both GLSLs and / 16


//Fluid sim development largely supported by raylib's 'game of life' example
//which shows how to use compute shaders in the context of raylib.
//Sorry, a good chunk of this was developed with the use of github copilot :(
//I was tired after I fully did it using compute shaders and then realized that mac doesn't even support them :(
void FluidSim::UpdateSim() {
    //float deltaTime = GetFrameTime();
    if (transferBuffer.count < MAXFLUIDSIMTRANSFERS) {
        if (type_ == type1) {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                float scaleX = (float)SIMWIDTH / (float)GetScreenWidth();
                float scaleY = (float)SIMHEIGHT / (float)GetScreenHeight();
                
                transferBuffer.commands[transferBuffer.count].x = (int)(GetMouseX() * scaleX);
                transferBuffer.commands[transferBuffer.count].y = (int)(GetMouseY() * scaleY);
                
                Vector2 delta = GetMouseDelta();
                transferBuffer.commands[transferBuffer.count].xVector = delta.x * scaleX;
                transferBuffer.commands[transferBuffer.count].yVector = delta.y * scaleY;

                transferBuffer.commands[transferBuffer.count].w = 10;

                transferBuffer.commands[transferBuffer.count].viscosity = 1.0f;

                transferBuffer.commands[transferBuffer.count].pressure = 0.2f;
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

    RenderTexture2D* inputPart1 = useStateA ? &stateA_part1 : &stateB_part1;
    RenderTexture2D* inputPart2 = useStateA ? &stateA_part2 : &stateB_part2;
    
    //transfer stuff into thing/run commands
    if (transferBuffer.count > 0) {
        float commandData[MAXFLUIDSIMTRANSFERS * 12]; //3 vec4s per command
        for (unsigned int i = 0; i < transferBuffer.count; i++) {
            unsigned int base = i * 12;
            commandData[base + 0] = (float)transferBuffer.commands[i].x;
            commandData[base + 1] = (float)transferBuffer.commands[i].y;
            commandData[base + 2] = (float)transferBuffer.commands[i].w;
            commandData[base + 3] = (float)transferBuffer.commands[i].xVector;
            
            commandData[base + 4] = (float)transferBuffer.commands[i].yVector;
            commandData[base + 5] = transferBuffer.commands[i].pressure;
            commandData[base + 6] = transferBuffer.commands[i].r;
            commandData[base + 7] = transferBuffer.commands[i].g;
            
            commandData[base + 8] = transferBuffer.commands[i].b;
            commandData[base + 9] = 0.0f;
            commandData[base + 10] = 0.0f;
            commandData[base + 11] = 0.0f;
        }
        

        SetShaderValue(fluidSimTransferShader, transferCountUniformLoc, &transferBuffer.count, SHADER_UNIFORM_UINT);
        int commandsLoc = GetShaderLocation(fluidSimTransferShader, "commands");
        SetShaderValueV(fluidSimTransferShader, commandsLoc, commandData, SHADER_UNIFORM_VEC4, MAXFLUIDSIMTRANSFERS * 3);
        
        int part2LocTransfer = GetShaderLocation(fluidSimTransferShader, "tileBufferPart2");
        
        // Manually bind Part2 to texture unit 1
        int unit = 1;
        rlActiveTextureSlot(1);
        rlEnableTexture(inputPart2->texture.id);
        rlSetUniform(part2LocTransfer, &unit, SHADER_UNIFORM_INT, 1);
        
        // Source rectangle (flip Y for render texture)
        Rectangle srcRec = {0, 0, (float)SIMWIDTH, -(float)SIMHEIGHT};
        Rectangle dstRec = {0, 0, (float)SIMWIDTH, (float)SIMHEIGHT};
        
        //run transfer shader - Pass 1 (output Part1)
        int out0 = 0;
        SetShaderValue(fluidSimTransferShader, transferOutputPartLoc, &out0, SHADER_UNIFORM_INT);
        BeginTextureMode(useStateA ? stateB_part1 : stateA_part1);
        BeginShaderMode(fluidSimTransferShader);
        DrawTexturePro(inputPart1->texture, srcRec, dstRec, (Vector2){0, 0}, 0, WHITE);
        EndShaderMode();
        EndTextureMode();

        // Pass 2 (output Part2)
        int out1 = 1;
        SetShaderValue(fluidSimTransferShader, transferOutputPartLoc, &out1, SHADER_UNIFORM_INT);
        BeginTextureMode(useStateA ? stateB_part2 : stateA_part2);
        BeginShaderMode(fluidSimTransferShader);
        DrawTexturePro(inputPart1->texture, srcRec, dstRec, (Vector2){0, 0}, 0, WHITE);
        EndShaderMode();
        EndTextureMode();
        
        rlActiveTextureSlot(0); // Reset to default
        
        transferBuffer.count = 0;
        useStateA = !useStateA; // Swap
    }


    //step simulation here
    BeginTextureMode(renderTexture1);
    BeginShaderMode(fluidSimLogicShader);
    DrawTexturePro(renderTexture2.texture, Rectangle{0,0,SIMWIDTH,SIMHEIGHT}, Rectangle{0,0,SIMWIDTH,SIMHEIGHT}, {0,0}, 0, GREEN);
    EndShaderMode();
    EndTextureMode();
    //now renderTexture1 has the correct state after logic is applied
    
}

void FluidSim::DrawSim() {
    BeginShaderMode(fluidSimRenderShader);
    Vector2 res{(float)GetScreenWidth(), (float)GetScreenHeight()};
    SetShaderValue(fluidSimRenderShader, rendererShaderResLoc, &res, SHADER_UNIFORM_VEC2);
    DrawTexturePro(renderTexture1.texture, {0,0,SIMWIDTH,SIMHEIGHT}, {0,0,(float)GetScreenWidth(), (float)GetScreenHeight()}, {0,0}, 0, PURPLE);
    EndShaderMode();
    
    rlActiveTextureSlot(0); // Reset to default
}

FluidSim::FluidSim(FluidSimType type) {
    type_ = type;
    useStateA = true;
    transferBuffer = {0};

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
    ClearBackground({0,0,0,0});
    EndTextureMode();
    BeginTextureMode(renderTexture2);
    ClearBackground({0,0,0,0});
    EndTextureMode();
    
    transferBuffer = {0};
    
    BeginTextureMode(stateA_part1);
    ClearBackground(BLANK);
    EndTextureMode();
    
    BeginTextureMode(stateA_part2);
    ClearBackground(BLANK);
    EndTextureMode();

    BeginTextureMode(stateB_part1);
    ClearBackground(BLANK);
    EndTextureMode();

    BeginTextureMode(stateB_part2);
    ClearBackground(BLANK);
    EndTextureMode();
}