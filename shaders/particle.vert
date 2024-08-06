#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aVel;
layout(location = 2) in float aMass;
layout(location = 3) in vec3 aColor;

out vec3 fragColor;

uniform float deltaTime;
uniform float G;
uniform int numParticles;

uniform vec2 positions[1000]; // Adjust based on your max particles
uniform float masses[1000];   // Adjust based on your max particles

void main() {
    vec2 force = vec2(0.0);

    for (int i = 0; i < numParticles; ++i) {
        vec2 dir = positions[i] - aPos;
        float distSquared = dot(dir, dir) + 0.01; // Avoid division by zero
        float invDist = inversesqrt(distSquared);
        force += G * aMass * masses[i] * dir * invDist * invDist * invDist;
    }

    vec2 newVel = aVel + deltaTime * force / aMass;
    vec2 newPos = aPos + deltaTime * newVel;

    fragColor = aColor;

    // Output new position for rendering
    gl_Position = vec4(newPos, 0.0, 1.0);
}
