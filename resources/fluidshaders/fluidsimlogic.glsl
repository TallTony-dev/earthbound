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
#define YVELLOSS (0.55f)
#define XVELLOSS (0.55f)
#define DIAGMULT (0.2f)

#define xVel(tile) (tile.z - 0.5)
#define yVel(tile) (tile.w - 0.5)

void main() {
    vec2 tileCoord = fragTexCoord * vec2(SIMWIDTH, SIMHEIGHT);
    vec4 tileState = texture(texture0, fragTexCoord);
    
    finalTileState = tileState;

    vec4 leftTile = texture(texture0, vec2(tileCoord.x - 1, tileCoord.y) / vec2(SIMWIDTH, SIMHEIGHT));
    vec4 rightTile = texture(texture0, vec2(tileCoord.x + 1, tileCoord.y) / vec2(SIMWIDTH, SIMHEIGHT));
    vec4 topTile = texture(texture0, vec2(tileCoord.x, tileCoord.y + 1) / vec2(SIMWIDTH, SIMHEIGHT));
    vec4 bottomTile = texture(texture0, vec2(tileCoord.x, tileCoord.y - 1) / vec2(SIMWIDTH, SIMHEIGHT));
    vec4 toprightTile = texture(texture0, vec2(tileCoord.x + 1, tileCoord.y + 1) / vec2(SIMWIDTH, SIMHEIGHT));
    vec4 topleftTile = texture(texture0, vec2(tileCoord.x - 1, tileCoord.y + 1) / vec2(SIMWIDTH, SIMHEIGHT));
    vec4 bottomrightTile = texture(texture0, vec2(tileCoord.x + 1, tileCoord.y - 1) / vec2(SIMWIDTH, SIMHEIGHT));
    vec4 bottomleftTile = texture(texture0, vec2(tileCoord.x - 1, tileCoord.y - 1) / vec2(SIMWIDTH, SIMHEIGHT));


    float velx = xVel(finalTileState);
    float vely = yVel(finalTileState);

    velx -= xVel(leftTile);
    velx -= xVel(topleftTile) * DIAGMULT;
    velx -= xVel(bottomleftTile) * DIAGMULT;

    velx += xVel(rightTile);
    velx += xVel(toprightTile) * DIAGMULT;
    velx += xVel(bottomrightTile) * DIAGMULT;

    vely -= yVel(bottomTile);
    vely -= yVel(bottomleftTile) * DIAGMULT;
    vely -= yVel(bottomrightTile) * DIAGMULT;

    vely += yVel(topTile);
    vely += yVel(topleftTile) * DIAGMULT;
    vely += yVel(toprightTile) * DIAGMULT;


    finalTileState.z = velx * XVELLOSS + 0.5;
    finalTileState.w = vely * YVELLOSS + 0.5;


    finalTileState.x += PRESSUREMULT * velx;
    finalTileState.x += PRESSUREMULT * vely;

}