#version 330 core

// fullscreen triangle trick
// no vbo needed  vertex id drives position
// covers the screen with one triangle which is faster than two

out vec2 vUV;

void main() {
    vec2 verts[3] = vec2[](
        vec2(-1.0, -1.0),
        vec2( 3.0, -1.0),
        vec2(-1.0,  3.0)
    );

    vec2 p = verts[gl_VertexID];
    vUV = (p + 1.0) * 0.5;
    gl_Position = vec4(p, 0.0, 1.0);
}
