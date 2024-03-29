#version 450

vec2 positions[3] = {
    vec2(-0.25, +0.25),
    vec2( 0.00, -0.25),
    vec2(+0.25, +0.25)
};

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    fragColor = vec3(0.5, 0.0, 0.0);
}
