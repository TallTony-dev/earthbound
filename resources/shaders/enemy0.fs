# version 330 core

#define PALETTECOUNT 4

precision mediump float;
//check config.h for what values are preset by raylib
// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;
uniform float totalTime;
uniform sampler2D texture0;

//uniform float dmgpercent;

out vec4 finalColor;



void main() {
    finalColor = vec4(0.4, 0.6, cos(totalTime) / 2 + 1, 0); 

    vec2 center = vec2(0.5,0.5);
    float dist = distance(fragTexCoord, center);


    vec2 scan = vec2(gl_FragCoord.xy);
    scan.y += 4 * cos(gl_FragCoord.x / 20 + totalTime * 2);
    scan.y += 2000 * sin(fragTexCoord.x * 3.14159) * sin(totalTime) * dist * dist;
    if (int(scan.y) % 6 == 0 || int(scan.y + 1) % 6 == 0)
        finalColor.rgb *= vec3(0.1,0.1,0.1);
    // else if (int(scan.y - 1) % 6 == 0)
    //     finalColor.rgb += vec3(0.2,0.2,0.2);

    
    finalColor.a += (dist-0.5) * (dist-0.5) * 10;
}