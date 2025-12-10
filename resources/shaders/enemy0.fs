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

uniform float timeSinceDamaged;

out vec4 finalColor;



void main() {
    finalColor = vec4(0.4, 0.6, cos(totalTime) / 2 + 1, 0); 

    vec2 center = vec2(0.5 + 0.2 * sin(totalTime),0.5 + 0.1 * cos(totalTime) * cos(totalTime * 0.85 + 1.3));

    float damageIntensity = 1 / (40 + pow(timeSinceDamaged, 10));
    center.y += sin(timeSinceDamaged * 100) * damageIntensity;

    float dist = distance(fragTexCoord - vec2((sin(fragTexCoord.y * 10 + fragTexCoord.x * 7) * damageIntensity), (fragTexCoord * damageIntensity)), center);


    vec2 scan = vec2(gl_FragCoord.xy);
    scan.y += 4 * cos(gl_FragCoord.x / 20 + totalTime * 2);
    scan.y -= dist * 4000 * (fragTexCoord.y - 0.5);
    if (int(scan.y/2) % 20 == 0 || int(scan.y/2 + 1) % 20 == 0)
        finalColor.rgb *= vec3(0.1,0.1,0.1);
    // else if (int(scan.y - 1) % 6 == 0)
    //     finalColor.rgb += vec3(0.2,0.2,0.2);
    finalColor.r += timeSinceDamaged;

    float value = (min((dist-0.5), 0.0)) * (min((dist-0.5), 0.0)) * 10 + 0.1 * cos(dist + totalTime);
    finalColor.a += value * step(0.5, value);
}