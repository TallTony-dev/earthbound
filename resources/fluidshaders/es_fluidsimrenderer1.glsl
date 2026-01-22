#version 300 es
precision highp float;

#define SIMWIDTH 2000.0
#define SIMHEIGHT 2000.0

in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

out vec4 finalColor;

uniform sampler2D texture0;

uniform vec2 resolution;

void main() {
    vec2 scales = vec2(resolution.x / SIMWIDTH, resolution.y / SIMHEIGHT);
    vec4 tile = texture(texture0, fragTexCoord * scales);
    finalColor = vec4(abs(tile.z - 0.5) * 2.0, 0.0, abs(tile.w - 0.5) * 2.0, tile.x);
}