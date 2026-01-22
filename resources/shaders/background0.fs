# version 330 core

#define PALETTECOUNT 4

precision mediump float;
//check config.h for what values are preset by raylib
// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

uniform float totalTime;
vec3 palettes[PALETTECOUNT] = vec3[](
    vec3(0.85, 0.32, 0.45), //rose
    vec3(0.28, 0.55, 0.88), //blue
    vec3(0.96, 0.78, 0.25), //yellow
    vec3(0.35, 0.82, 0.48)  //green
);

#define SPEED 3

// Input uniform values
uniform sampler2D texture0;

out vec4 finalColor;


//sobel adapted from raylib examples
vec3 Sobel(vec2 uv) {
    float x = 1.0/416;
    float y = 1.0/312;

    vec4 horizEdge = vec4(0.0);
    horizEdge -= texture(texture0, fract(vec2(uv.x - x, uv.y - y)))*1.0;
    horizEdge -= texture(texture0, fract(vec2(uv.x - x, uv.y   )))*2.0;
    horizEdge -= texture(texture0, fract(vec2(uv.x - x, uv.y + y)))*1.0;
    horizEdge += texture(texture0, fract(vec2(uv.x + x, uv.y - y)))*1.0;
    horizEdge += texture(texture0, fract(vec2(uv.x + x, uv.y   )))*2.0;
    horizEdge += texture(texture0, fract(vec2(uv.x + x, uv.y + y)))*1.0;

    vec4 vertEdge = vec4(0.0);
    vertEdge -= texture(texture0, fract(vec2(uv.x - x, uv.y - y)))*1.0;
    vertEdge -= texture(texture0, fract(vec2(uv.x    , uv.y - y)))*2.0;
    vertEdge -= texture(texture0, fract(vec2(uv.x + x, uv.y - y)))*1.0;
    vertEdge += texture(texture0, fract(vec2(uv.x - x, uv.y + y)))*1.0;
    vertEdge += texture(texture0, fract(vec2(uv.x    , uv.y + y)))*2.0;
    vertEdge += texture(texture0, fract(vec2(uv.x + x, uv.y + y)))*1.0;

    vec3 edge = sqrt((horizEdge.rgb*horizEdge.rgb) + (vertEdge.rgb*vertEdge.rgb));
    return edge;

    //finalColor = vec4(edge, texture(texture0, fragTexCoord).a);
}


void main() {

    vec2 sinTexCoord = fragTexCoord.xy + sin(fragTexCoord.y * 4 + totalTime / 3) / 50;

    int currentPaletteIndex = int(mod(floor(totalTime/2), PALETTECOUNT));
    vec3 palette = palettes[currentPaletteIndex];

    float sinTime = sin(totalTime * SPEED);

    float func1 = cos(sinTime + sinTexCoord.y * 1000) / 2.0 + 0.5 - cos(sinTexCoord.y * 100 + totalTime * SPEED) + sin(sinTexCoord.x * 1000 + totalTime) + 0.2;
    float func2 = sin(sinTime + fragTexCoord.x * 400) / 2.0 - sinTime * cos(totalTime) - sin(fragTexCoord.y * 400 + totalTime * SPEED) + 0.2;
    finalColor = vec4(func1 + func2 * palette.r,func2 * palette.g + 0.8 * func1,0.4 + 0.3 * func1 + func2 * palette.g,1);

    finalColor.rgb *= clamp(palette.rgb * 0.5 + 0.4, 0.0, 1.0);
    if (int(gl_FragCoord.y) % 6 == 0 || int(gl_FragCoord.y + 1) % 6 == 0)
        finalColor.rgb *= vec3(0.2,0.2,0.2);
    else if (int(gl_FragCoord.y - 1) % 6 == 0)
        finalColor.rgb += vec3(0.2,0.2,0.2);

    vec2 uv = fragTexCoord;
    uv.y += 1 * sin(fragTexCoord.x + totalTime * 0.4);
    uv.xy /= 2 + 1.5 * sin(totalTime * 0.15); //zoom

    uv.x -= (1 - fragTexCoord.y) * (-fragTexCoord.x + 0.5) * (sin(totalTime * 0.2) * 0.5 + 0.5); //convergence

    uv.x += 0.4 * sin(fragTexCoord.y + totalTime * 0.2) + sin(totalTime * 0.05);
    float edge = Sobel(uv).r;
    finalColor.rgb -= edge * 4;
}
