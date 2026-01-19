#version 330 core

#define SIMWIDTH 1200
#define SIMHEIGHT 800

out vec4 finalColor;

uniform sampler2D texture0;

uniform vec2 resolution;

// struct FluidTile {
//     float pressure;
//     float viscosity;
//     float velocityX;
//     float velocityY;
// };


void main() {
    vec2 scales = vec2(resolution.x / SIMWIDTH, resolution.y / SIMHEIGHT);
    vec4 tile = texture(texture0, fragTexCoord * scales);
    finalColor = vec4(tile.y, tile.w, tile.z, 1.0);
}