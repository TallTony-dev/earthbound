#version 300 es

#define PALETTECOUNT 4

precision highp float;
//check config.h for what values are preset by raylib

in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;
uniform float totalTime;
uniform sampler2D texture0;

uniform float timeSinceDamaged;

out vec4 finalColor;



void main() {
    finalColor = vec4(0.4, 0.6, cos(totalTime) / 2.0 + 1.0, 0.0); 

    vec2 center = vec2(0.5 + 0.2 * sin(totalTime),0.5 + 0.1 * cos(totalTime) * cos(totalTime * 0.85 + 1.3));

    float damageIntensity = 1.0 / (40.0 + pow(timeSinceDamaged, 10.0));
    center.y += sin(timeSinceDamaged * 100.0) * damageIntensity;

    float dist = distance(fragTexCoord - vec2((sin(fragTexCoord.y * 10.0 + fragTexCoord.x * 7.0) * damageIntensity), (fragTexCoord.x * damageIntensity)), center);


    vec2 scan = vec2(gl_FragCoord.xy);
    scan.y += 4.0 * cos(gl_FragCoord.x / 20.0 + totalTime * 2.0);
    scan.y -= dist * 4000.0 * (fragTexCoord.y - 0.5);
    if (mod(scan.y/2.0, 20.0) < 1.0 || mod(scan.y/2.0 + 1.0, 20.0) < 1.0)
        finalColor.rgb *= vec3(0.1,0.1,0.1);
    finalColor.r += timeSinceDamaged;

    float value = (min((dist-0.5), 0.0)) * (min((dist-0.5), 0.0)) * 10.0 + 0.1 * cos(dist + totalTime);
    finalColor.a += value * step(0.5, value);
}