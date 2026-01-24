#version 330 core

#define SIMWIDTH 2000 //MUST MATCH fluidsim.cpp
#define SIMHEIGHT 2000 //MUST MATCH fluidsim.cpp
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
        float pressure = commands[i * 7 + 5];
        float viscosity = commands[i * 7 + 6];
        float dist = distance(tileCoord, vec2(x,y));
        if (dist <= radius) { //if this tile is in the radius
            float deltax = x - tileCoord.x;
            float deltay = y - tileCoord.y;

            finalSimState.x += pressure;

            float velx = ((finalSimState.z - 0.5) * 2 + float(xVector) * VECTORSTRENGTHMULT) / (deltax / 15 + 1 * sign(deltax));
            float vely = ((finalSimState.w - 0.5) * 2 + float(yVector) * VECTORSTRENGTHMULT) / (deltay / 15 + 1 * sign(deltay));
            velx = clamp(velx, -1.0, 1.0);
            vely = clamp(vely, -1.0, 1.0);
            finalSimState.z = velx * 0.5 + 0.50001;
            finalSimState.w = vely * 0.5 + 0.50001;


            float partialPressure = (pressure + finalSimState.x) / max(finalSimState.x, 0.0001);
            finalSimState.y = partialPressure * viscosity;
            
        }
    }
    //finalSimState.w = max(finalSimState.a, 0.00001);
}