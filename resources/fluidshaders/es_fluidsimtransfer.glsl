#version 300 es
precision highp float;

#define SIMWIDTH 2000.0
#define SIMHEIGHT 2000.0
#define MAXFLUIDSIMTRANSFERS 6

uniform sampler2D texture0;

uniform float commands[MAXFLUIDSIMTRANSFERS * 7];
uniform int commandCount;

in vec2 fragTexCoord;
out vec4 finalSimState;

#define VECTORSTRENGTHMULT 0.3

void main() {
    vec2 tileCoord = fragTexCoord * vec2(SIMWIDTH, SIMHEIGHT);

    finalSimState = texture(texture0, fragTexCoord);

    for (int i = 0; i < commandCount; i++) {
        float x = commands[i * 7 + 0];
        float y = commands[i * 7 + 1];
        float radius = commands[i * 7 + 2];
        float xVector = commands[i * 7 + 3];
        float yVector = commands[i * 7 + 4];
        float pressure = 0.0;
        float viscosity = commands[i * 7 + 6];

        if (distance(tileCoord, vec2(x,y)) <= radius) {
            finalSimState.x += pressure;

            float velx = (finalSimState.z - 0.5) * 2.0 + xVector * VECTORSTRENGTHMULT;
            float vely = (finalSimState.w - 0.5) * 2.0 + yVector * VECTORSTRENGTHMULT;
            velx = clamp(velx, -1.0, 1.0);
            vely = clamp(vely, -1.0, 1.0);
            finalSimState.z = velx * 0.5 + 0.50001;
            finalSimState.w = vely * 0.5 + 0.50001;

            float partialPressure = (pressure + finalSimState.x) / max(finalSimState.x, 0.0001);
            finalSimState.y = partialPressure * viscosity;
        }
    }
}