#version 330 core

#define SIMWIDTH 1200 //MUST MATCH fluidsim.cpp
#define SIMHEIGHT 1200 //MUST MATCH fluidsim.cpp
#define MAXFLUIDSIMTRANSFERS 6

// struct FluidTile {
//     float pressure;
//     float viscosity;
//     float velocityX;
//     float velocityY;
// };

uniform sampler2D texture0; //previous fluidsimstate


uniform float commands[MAXFLUIDSIMTRANSFERS * 7];
uniform int commandCount;

in vec2 fragTexCoord;
out vec4 finalSimState;


#define VECTORSTRENGTHMULT (0.3f)

void main() {
    vec2 tileCoord = fragTexCoord * vec2(SIMWIDTH, SIMHEIGHT);

    finalSimState = texture(texture0, fragTexCoord);
    //pressure = x, viscosity = y, velocityx = z, velocityy = w

    for (int i = 0; i < commandCount; i++) { //check all commands
        float x = commands[i * 7 + 0];
        float y = commands[i * 7 + 1];
        int radius = int(commands[i * 7 + 2]);
        float xVector = commands[i * 7 + 3];
        float yVector = commands[i * 7 + 4];
        float pressure = /*commands[i * 7 + 5]*/0;
        float viscosity = commands[i * 7 + 6];

        if (abs(tileCoord.x - x) <= float(radius) && abs(tileCoord.y - y) <= float(radius)) { //if this tile is in the radius
            finalSimState.x += pressure;

            float velx = (finalSimState.z - 0.5) + float(xVector) * VECTORSTRENGTHMULT;
            float vely = (finalSimState.w - 0.5) + float(yVector) * VECTORSTRENGTHMULT;

            finalSimState.z = velx + 0.5;
            finalSimState.w = vely + 0.5;


            float partialPressure = (pressure + finalSimState.x) / max(finalSimState.x, 0.0001);
            finalSimState.y = partialPressure * viscosity;
            
        }
    }
    //finalSimState.w = max(finalSimState.a, 0.00001);
}