#version 330 core

#define SIMWIDTH 1200
#define SIMHEIGHT 1200

// struct FluidTile {
//     float pressure;
//     float viscosity;
//     float velocityX;
//     float velocityY;
// };

in vec2 fragTexCoord;

uniform sampler2D texture0;

out vec4 finalTileState;

#define PRESSUREMULT (0.1f)
#define VELOCITYMULT (1.0f)
#define VISCOSITY (0.0001f)

#define xVel(tile) ((tile.z - 0.5) * 2.0)
#define yVel(tile) ((tile.w - 0.5) * 2.0)
#define Vel(tile) (vec2(xVel(tile), yVel(tile)))


//NOTE: SOME LLM USAGE HERE, I made sure to not just copy paste, and to understand what I was doing
//Some of these fluid dynamics calcs were a bit beyond my knowledge
void main() {
    vec2 tileCoord = fragTexCoord * vec2(SIMWIDTH, SIMHEIGHT);
    vec2 texelSize = 1.0 / vec2(SIMWIDTH, SIMHEIGHT);

    vec4 tileState = texture(texture0, fragTexCoord);
    vec2 vel = Vel(tileState);

    vec4 leftTile = texture(texture0, vec2(tileCoord.x - 1, tileCoord.y) * texelSize);
    vec4 rightTile = texture(texture0, vec2(tileCoord.x + 1, tileCoord.y) * texelSize);
    vec4 topTile = texture(texture0, vec2(tileCoord.x, tileCoord.y + 1) * texelSize);
    vec4 bottomTile = texture(texture0, vec2(tileCoord.x, tileCoord.y - 1) * texelSize);

    vec2 advectedCoord = /*ceil*/((tileCoord - vel) * VELOCITYMULT);
    vec4 advectedTileState = texture(texture0, advectedCoord * texelSize);
    vec2 advectedVel = Vel(advectedTileState);

    vec2 leftVel = Vel(leftTile);
    vec2 rightVel = Vel(rightTile);
    vec2 topVel = Vel(topTile);
    vec2 bottomVel = Vel(bottomTile);

    vec2 laplacian = (leftVel + rightVel + topVel + bottomVel) - 4.0 * advectedVel;
    vec2 diffusedVel = advectedVel + VISCOSITY * laplacian;

    //how much would be abnormally created at this spot and must be accounted for
    float divergence = (rightVel.x - leftVel.x + topVel.y - bottomVel.y) * 0.5;

    float pressure = (rightTile.x + leftTile.x + topTile.x + bottomTile.x - divergence) * 0.25;

    vec2 pressureGrad = vec2(rightTile.x - leftTile.x, topTile.x - bottomTile.x) * 0.5;
    vec2 finalVel = diffusedVel - pressureGrad;
    finalVel = clamp(finalVel, vec2(-1.0), vec2(1.0));

    finalTileState.x = pressure;
    finalTileState.y = advectedTileState.y;
    finalTileState.z = finalVel.x * 0.5 + 0.5;
    finalTileState.w = finalVel.y * 0.5 + 0.5, 0.0001;
}