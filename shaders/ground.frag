#version 330 core

in vec3 vWorld;
out vec4 FragColor;

float grid(vec2 coord, float scale) {
    vec2 p = coord * scale;
    vec2 g = abs(fract(p - 0.5) - 0.5) / fwidth(p);
    float line = min(g.x, g.y);
    return 1.0 - min(line, 1.0);
}

void main() {
    // simple grid for spatial reference
    float minor = grid(vWorld.xz, 1.0);
    float major = grid(vWorld.xz, 0.2);

    vec3 base = vec3(0.06, 0.07, 0.08);
    vec3 gcol = vec3(0.16, 0.18, 0.20);

    vec3 col = base + gcol * (0.10 * minor + 0.25 * major);

    FragColor = vec4(col, 1.0);
}
