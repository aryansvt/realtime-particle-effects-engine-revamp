#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uScene;
uniform float uThreshold;
uniform float uKnee;

// soft knee threshold from cod advanced warfare and others
// gives a smoother rolloff than a hard cutoff
vec3 quadratic_threshold(vec3 col, float threshold, float knee) {
    float br = max(col.r, max(col.g, col.b));

    // soft knee region
    float soft = br - threshold + knee;
    soft = clamp(soft, 0.0, 2.0 * knee);
    soft = soft * soft / (4.0 * knee + 0.0001);

    float contribution = max(soft, br - threshold);
    contribution /= max(br, 0.0001);

    return col * contribution;
}

void main() {
    vec3 col = texture(uScene, vUV).rgb;
    FragColor = vec4(quadratic_threshold(col, uThreshold, 0.5), 1.0);
}
