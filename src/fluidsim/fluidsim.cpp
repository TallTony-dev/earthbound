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
    float deltaTime = GetFrameTime();
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

                transferBuffer.commands[transferBuffer.count].w = 6.0f;

                float t = (float)GetTime();
                transferBuffer.commands[transferBuffer.count].r = 0.5f + 0.5f * sinf(t * 0.7f);
                transferBuffer.commands[transferBuffer.count].g = 0.5f;
                transferBuffer.commands[transferBuffer.count].b = 0.5f + 0.5f * sinf(t * 1.6f + 4.0f);

                transferBuffer.commands[transferBuffer.count].pressure = 2.0f;
                transferBuffer.count++;
            }
        }
    }

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

    //simulate stuff
    inputPart1 = useStateA ? &stateA_part1 : &stateB_part1;
    inputPart2 = useStateA ? &stateA_part2 : &stateB_part2;
    
    int part2LocLogic = GetShaderLocation(fluidSimLogicShader, "tileBufferPart2");
    
    // Manually bind Part2 to texture unit 1
    int unitLogic = 1;
    rlActiveTextureSlot(1);
    rlEnableTexture(inputPart2->texture.id);
    rlSetUniform(part2LocLogic, &unitLogic, SHADER_UNIFORM_INT, 1);
    
    Rectangle srcRec = {0, 0, (float)SIMWIDTH, -(float)SIMHEIGHT};
    Rectangle dstRec = {0, 0, (float)SIMWIDTH, (float)SIMHEIGHT};
    
    // Logic Pass 1
    int outL0 = 0;
    SetShaderValue(fluidSimLogicShader, logicOutputPartLoc, &outL0, SHADER_UNIFORM_INT);
    BeginTextureMode(useStateA ? stateB_part1 : stateA_part1);
    BeginShaderMode(fluidSimLogicShader);
    DrawTexturePro(inputPart1->texture, srcRec, dstRec, (Vector2){0, 0}, 0, WHITE);
    EndShaderMode();
    EndTextureMode();

    // Logic Pass 2
    int outL1 = 1;
    SetShaderValue(fluidSimLogicShader, logicOutputPartLoc, &outL1, SHADER_UNIFORM_INT);
    BeginTextureMode(useStateA ? stateB_part2 : stateA_part2);
    BeginShaderMode(fluidSimLogicShader);
    DrawTexturePro(inputPart1->texture, srcRec, dstRec, (Vector2){0, 0}, 0, WHITE);
    EndShaderMode();
    EndTextureMode();
    
    rlActiveTextureSlot(0); // Reset to default
    
    useStateA = !useStateA;
}

void FluidSim::DrawSim() {
    RenderTexture2D* currentPart1 = useStateA ? &stateA_part1 : &stateB_part1;
    RenderTexture2D* currentPart2 = useStateA ? &stateA_part2 : &stateB_part2;
    
    Vector2 resolution = {(float)GetScreenWidth(), (float)GetScreenHeight()};
    SetShaderValue(fluidSimRenderShader, resUniformLoc, &resolution, SHADER_UNIFORM_VEC2);
    
    // Manually bind Part2 to texture unit 1
    int part2Loc = GetShaderLocation(fluidSimRenderShader, "tileBufferPart2");
    int unit = 1;
    rlActiveTextureSlot(1);
    rlEnableTexture(currentPart2->texture.id);
    rlSetUniform(part2Loc, &unit, SHADER_UNIFORM_INT, 1);
    
    BeginShaderMode(fluidSimRenderShader);
    // Draw Part1 texture - it will be bound to texture0 automatically by raylib
    Rectangle srcRec = {0, 0, (float)SIMWIDTH, -(float)SIMHEIGHT}; // Flip Y for RenderTexture
    Rectangle dstRec = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
    DrawTexturePro(currentPart1->texture, srcRec, dstRec, (Vector2){0, 0}, 0, WHITE);
    EndShaderMode();
    
    rlActiveTextureSlot(0); // Reset to default
}

FluidSim::FluidSim(FluidSimType type) {
    type_ = type;
    useStateA = true;
    transferBuffer = {0};

    // Load shaders
    fluidSimTransferShader = LoadShader(0, TextFormat("%s%s", GetApplicationDirectory(), "../resources/fluidshaders/fluidsimtransfer.glsl"));
    fluidSimLogicShader = LoadShader(0, TextFormat("%s%s", GetApplicationDirectory(), "../resources/fluidshaders/fluidsimlogic.glsl"));
    fluidSimRenderShader = LoadShader(0, TextFormat("%s%s", GetApplicationDirectory(), "../resources/fluidshaders/fluidsimrenderer1.glsl"));
    
    transferCountUniformLoc = GetShaderLocation(fluidSimTransferShader, "commandCount");
    transferOutputPartLoc = GetShaderLocation(fluidSimTransferShader, "outputPart");
    logicOutputPartLoc = GetShaderLocation(fluidSimLogicShader, "outputPart");
    resUniformLoc = GetShaderLocation(fluidSimRenderShader, "resolution");

    // Create render textures for ping-pong (2 sets, 2 textures each for MRT)
    stateA_part1 = LoadRenderTexture(SIMWIDTH, SIMHEIGHT);
    stateA_part2 = LoadRenderTexture(SIMWIDTH, SIMHEIGHT);
    stateB_part1 = LoadRenderTexture(SIMWIDTH, SIMHEIGHT);
    stateB_part2 = LoadRenderTexture(SIMWIDTH, SIMHEIGHT);
    
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