#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aVel;
layout(location = 2) in float aMass;
layout(location = 3) in vec3 aColor;

out vec3 fragColor;

void main() {
    gl_PointSize = aMass * 5.0;  // Adjust the multiplier for desired size
    fragColor = aColor;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
