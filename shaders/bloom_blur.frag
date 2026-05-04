#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uTex;
uniform vec2 uTexel;        // 1.0 / texture size
uniform int uHorizontal;    // 1 = horizontal pass  0 = vertical pass

// 9 tap gaussian kernel weights
const float weights[5] = float[](
    0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216
);

void main() {
    vec3 result = texture(uTex, vUV).rgb * weights[0];

    if (uHorizontal == 1) {
        for (int i = 1; i < 5; i++) {
            float off = float(i) * uTexel.x;
            result += texture(uTex, vUV + vec2(off, 0.0)).rgb * weights[i];
            result += texture(uTex, vUV - vec2(off, 0.0)).rgb * weights[i];
        }
    } else {
        for (int i = 1; i < 5; i++) {
            float off = float(i) * uTexel.y;
            result += texture(uTex, vUV + vec2(0.0, off)).rgb * weights[i];
            result += texture(uTex, vUV - vec2(0.0, off)).rgb * weights[i];
        }
    }

    FragColor = vec4(result, 1.0);
}
