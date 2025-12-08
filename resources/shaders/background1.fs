# version 330 core
precision mediump float;
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
  float l2 = length(w - v) * length(w - v);  // i.e. |w-v|^2 -  avoid a sqrt
  if (l2 == 0.0) return p - v;   // v == w case
  // Consider the line extending the segment, parameterized as v + t (w - v).
  // We find projection of point p onto the line. 
  // It falls where t = [(p-v) . (w-v)] / |w-v|^2
  // We clamp t from [0,1] to handle points outside the segment vw.
  float t = max(0, min(1, dot(p - v, w - v) / l2));
  vec2 projection = v + t * (w - v);  // Projection falls on the segment
  return (p - projection);
}

void main() {
    vec2 fixedMousePos = mousePos.xy * 2;
    vec2 LMousePos = laggardMousePos.xy * 2;

    vec2 minvec = minimumVector(LMousePos, fixedMousePos, vec2(gl_FragCoord.x, gl_FragCoord.y));
    float lineLength = length(LMousePos - fixedMousePos);
    float distance = length(minvec);
    vec2 lineDirection = normalize(LMousePos - fixedMousePos);
    vec2 directionToLine = normalize(minvec);
    
    vec2 uv = fragTexCoord;
    uv.xy += (distance * sin(distance * 0.1 - totalTime * 20)) / (1000 * distance) - lineDirection.xy * length(lineDirection + directionToLine) * lineLength / ((distance + 30) * 2) / 10000;

    finalColor = texture(texture0, uv);
    //finalColor.rb += (distance * sin(distance * 0.1 - totalTime * 20)) / (10 * distance) - lineDirection.xy * length(lineDirection + directionToLine) * lineLength / ((distance + 30) * 2) / 10;
}