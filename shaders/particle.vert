#version 330 core

layout(location = 0) in vec2 aCorner;
layout(location = 1) in vec2 aUV;

// instance data
layout(location = 2) in vec3 iPos;
layout(location = 3) in float iSize;
layout(location = 4) in vec4 iColor;
layout(location = 5) in float iRotation;
layout(location = 6) in float iTile;
layout(location = 7) in vec3 iVel;

uniform mat4 uView;
uniform mat4 uProj;

uniform vec3 uCamRight;
uniform vec3 uCamUp;
uniform vec3 uCamForward;
uniform vec2 uTileScale;

// 0 = pure camera facing billboard
// > 0 = stretch quad along projected velocity by this much per unit speed
uniform float uVelStretch;

out vec2 vUV;
out vec4 vColor;
out float vViewZ;
flat out int vTile;

mat2 rot2(float a) {
    float c = cos(a);
    float s = sin(a);
    return mat2(c, -s, s, c);
}

void main() {
    vec3 world;

    if (uVelStretch > 0.0001 && dot(iVel, iVel) > 0.0001) {
        // anisotropic billboard aligned to velocity
        // project velocity onto the camera screen plane and use it as the long axis
        // perpendicular axis stays in the screen plane via cross with view direction
        vec3 vScreen = iVel - dot(iVel, uCamForward) * uCamForward;
        float vLen = length(vScreen);

        if (vLen > 0.0001) {
            vec3 vDir = vScreen / vLen;
            vec3 sideDir = normalize(cross(vDir, uCamForward));

            float speed = length(iVel);
            float stretch = iSize * (1.0 + speed * uVelStretch);

            // aCorner.y becomes length axis  aCorner.x becomes width axis
            world = iPos + sideDir * aCorner.x * iSize + vDir * aCorner.y * stretch;
        } else {
            // velocity points straight at camera fall back to billboard
            vec2 corner = rot2(iRotation) * aCorner;
            world = iPos + (uCamRight * corner.x + uCamUp * corner.y) * iSize;
        }
    } else {
        // standard camera facing billboard with rotation
        vec2 corner = rot2(iRotation) * aCorner;
        world = iPos + (uCamRight * corner.x + uCamUp * corner.y) * iSize;
    }

    vec4 viewPos = uView * vec4(world, 1.0);
    vViewZ = -viewPos.z;
    gl_Position = uProj * viewPos;

    vColor = iColor;
    vUV = aUV;
    vTile = int(iTile + 0.5);
}
