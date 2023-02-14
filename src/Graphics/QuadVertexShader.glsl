#version 330 core
layout (location = 0) in vec3 aPos;

out vec2 z;

uniform float aspRatio;
uniform float zoom;
uniform vec2 centre;

void main(){
    gl_Position = vec4(aPos, 1.0);
    z = aPos.xy;
    z.x *= aspRatio;

    z = z/zoom + centre;
}