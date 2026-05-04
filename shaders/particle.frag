#version 330 core

in vec2 vUV;
in vec4 vColor;
in float vViewZ;
flat in int vTile;

uniform sampler2D uAtlas;
uniform sampler2D uSceneDepth;

uniform vec2 uTileScale;
uniform vec2 uViewport;
uniform vec2 uDepthRange; // x = near  y = far

uniform float uSoftFade;  // fade distance in view space units  0 disables soft particles

out vec4 FragColor;

float linearize_depth(float ndc, float n, float f) {
    // ndc in [0 1] from depth texture sample
    float z = ndc * 2.0 - 1.0;
    return (2.0 * n * f) / (f + n - z * (f - n));
}

void main() {
    vec2 uv = vUV;
    uv.x = uv.x * uTileScale.x + float(vTile) * uTileScale.x;

    vec4 tex = texture(uAtlas, uv);

    // use the atlas mainly for shape and alpha
    float a = tex.a * vColor.a;

    // soft particles: sample scene depth and fade where the particle approaches geometry
    if (uSoftFade > 0.0001) {
        vec2 screenUV = gl_FragCoord.xy / uViewport;
        float sceneDepthNDC = texture(uSceneDepth, screenUV).r;
        float sceneZ = linearize_depth(sceneDepthNDC, uDepthRange.x, uDepthRange.y);

        // vViewZ is positive distance from camera in view space
        float dz = sceneZ - vViewZ;
        float fade = clamp(dz / uSoftFade, 0.0, 1.0);
        a *= fade;
    }

    if (a < 0.01) discard;

    // let the ui color fully drive the visible tint
    vec3 rgb = vColor.rgb;

    FragColor = vec4(rgb, a);
}
