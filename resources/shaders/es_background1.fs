#version 300 es
precision highp float;
//check config.h for what values are preset by raylib
// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

uniform sampler2D texture0;
uniform vec2 mousePos;
uniform vec2 laggardMousePos;

out vec4 finalColor;
uniform float totalTime;

//adapted from https://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment
vec2 minimumVector(vec2 v, vec2 w, vec2 p) {
  // Return minimum distance between line segment vw and point p
  float l2 = length(w - v) * length(w - v);
  if (l2 == 0.0) return p - v;
  float t = max(0.0, min(1.0, dot(p - v, w - v) / l2));
  vec2 projection = v + t * (w - v);
  return (p - projection);
}

void main() {
    vec2 fixedMousePos = mousePos.xy * 2.0;
    vec2 LMousePos = laggardMousePos.xy * 2.0;

    vec2 minvec = minimumVector(LMousePos, fixedMousePos, vec2(gl_FragCoord.x, gl_FragCoord.y));
    float lineLength = length(LMousePos - fixedMousePos);
    float distance = length(minvec);
    vec2 lineDirection = normalize(LMousePos - fixedMousePos);
    vec2 directionToLine = normalize(minvec);
    
    vec2 uv = fragTexCoord;
    uv.xy += (distance * sin(distance * 0.1 - totalTime * 20.0)) / (1000.0 * distance) - lineDirection.xy * length(lineDirection + directionToLine) * lineLength / ((distance + 30.0) * 2.0) / 10000.0;

    // Use fract() to manually repeat texture - WebGL doesn't support repeat on NPOT textures
    finalColor = texture(texture0, fract(uv));
}