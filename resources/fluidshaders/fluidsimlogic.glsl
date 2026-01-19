#version 330 core

#define SIMWIDTH 1200
#define SIMHEIGHT 800

// struct FluidTile {
//     float pressure;
//     float viscosity;
//     float velocityX;
//     float velocityY;
// };

in vec2 fragTexCoord;

uniform sampler2D texture0;

out vec4 finalTileState;

void main() {
    vec2 tileCoord = fragTexCoord * vec2(SIMWIDTH, SIMHEIGHT);
    vec4 tileState = texture(texture0, fragTexCoord);

    finalTileState = tileState;

}